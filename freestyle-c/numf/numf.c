#include <assert.h>
#include <bsd/libutil.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <math.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "numf.h"

static u64 g_recursive = 0;              // directory scan mode during index build
static u64 g_min_value = 1;              // range start (inclusive)
static u64 g_max_value = 1000;           // range end (exclusive)
static u64 g_interval = 600;             // max number of seconds between indexing
static int g_dir_parms_pos = 1;          // cmdline position of the first directory
static int g_indexer_keep_alive = 1;     // indexer control flag
static int g_indexer_processing = 0;     // indexer builder control flag
static int g_fake_eof = 0;               // getchar() interrupted by signal returns fake EOF
static int g_query_processing = 0;       // do not overwrite cache during query

static char** g_argv;                    // cmdline tokens
static pid_t* g_indexers;                // indexers pids
static int g_indexer_id;                 // indexer's positional identifier
static int g_indexers_count;             // number of indexers
static int g_number_max_len;             // max digits of number in range
static int g_number_min_len;             // min digits of number in range
static u64 g_value_range_size;           // just g_max_value - g_min_value
static const char* g_current_dir;        // indexer current directory
static pthread_t g_indexer_worker;       // indexing thread
static char g_index_path[PATH_MAX];      // buffer for generating index tempfile name
static char g_inode_path[PATH_MAX];      // buffer for converting inode to filepath
static struct timeval g_last_indexing;   // last indexing time
static struct index_t** g_index_arr;     // array of active index instances for each dir
static struct index_t* g_building_index; // next index instance

static void index_store(const u64 number, const ino_t inode, const u64 offset)
{
    struct num_trace_t* trace = g_building_index->traces[number - g_min_value];
    if (trace == NULL) {
        const u64 initial_capacity = 3;
        const u64 initial_bytes = sizeof(struct num_trace_t) + initial_capacity * sizeof(struct record_t);
        trace = calloc(initial_bytes, 1);
        trace->capacity = initial_capacity;
        g_building_index->traces[number - g_min_value] = trace;
        ++g_building_index->traces_count;
    } else if (trace->size == trace->capacity) {
        const u64 new_capacity = 2 * trace->capacity + 1;
        struct num_trace_t* bigger = realloc(trace, sizeof(struct num_trace_t) + new_capacity * sizeof(struct record_t));
        if (bigger == NULL)
            ERREXIT("Index realloc failed");
        trace = bigger;
        trace->capacity = new_capacity;
        g_building_index->traces[number - g_min_value] = trace;
    }

    DBGPRN("Storing %llu occurence of %llu found in %llu at offset %llu, slot=%llu from %llu", trace->size, number, (u64)inode, offset, number - g_min_value, g_min_value);
    ++g_building_index->records_count;
    trace->records[trace->size].file = inode;
    trace->records[trace->size].position = offset;
    ++trace->size;
}

static void index_file(const char* filepath)
{
    struct stat fs;
    int fd = open(filepath, O_RDONLY);
    if (fd <= 0) return; // project decision: ignore no access
    fstat(fd, &fs);
    char* buf = mmap(NULL, fs.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if (buf == NULL) return; // project decision: ignore no access
    DBGPRN("Indexing %s of size %lu, %i", filepath, fs.st_size, fd);
    char* begin = buf;
    char* end;
    while (1) {
        while (begin - buf < fs.st_size && !isdigit(*begin))
            ++begin;
        end = begin;
        while (end - buf < fs.st_size && isdigit(*end))
            ++end;

        if (end - begin >= g_number_min_len)
            for (int len = g_number_min_len; len <= g_number_max_len; ++len)
                for (char* pos = begin; pos + len <= end; ++pos) {
                    u64 num = 0;
                    for (int off = 0; off < len; ++off) 
                        num = 10 * num + *(pos + off) - '0';
                    if (g_min_value <= num && num < g_max_value)
                        index_store(num, fs.st_ino, pos - buf);
                }

        begin = end;
        if (begin - buf == fs.st_size)
            break;
    }

    munmap(buf, fs.st_size);
}

static size_t index_serialize(const int fd)
{
    size_t ret;
    size_t addr_size = 1;
    while (addr_size < 16) {
        const u64 header_size = sizeof(struct index_t);
        const u64 toc_max_size = (2 * g_building_index->traces_count + 1) * addr_size;
        const u64 payload_size = g_building_index->traces_count * addr_size + g_building_index->records_count * sizeof(struct record_t);
        const u64 max_addr_value = 1ULL << (8 * addr_size);
        if (header_size + toc_max_size + payload_size < max_addr_value && g_building_index->range_size < max_addr_value)
            break;
        addr_size <<= 1;
    }
    if (addr_size > 8)
        ERREXIT("For this you need an x%li CPU", addr_size * 8);

    g_building_index->address_size = addr_size;
    const size_t toc_size = g_value_range_size * addr_size;
    ret = write(fd, g_building_index, sizeof(struct index_t));
    DBGPRN("Writing header of size %lu, traces_count=%llu, records_count=%llu, addr_size: %lu", ret, g_building_index->traces_count, g_building_index->records_count, addr_size);
    // write toc
    u64 packed_size = 0;
    u64 content_size = 0;
    char* toc = malloc(toc_size);
    // precompute packed_size and content_size
    for (u64 i = 0; i < g_value_range_size; ++packed_size)
        if (g_building_index->traces[i] != NULL) {
            memcpy(toc + packed_size * addr_size, (char*)&content_size, addr_size);
            content_size += addr_size + g_building_index->traces[i]->size * sizeof(struct record_t);
            DBGPRN("Index region for %llu, toc_offset: %lli start, traces[i]->size=%llu content_size=%llu", g_min_value + i, packed_size * addr_size, g_building_index->traces[i]->size, content_size);
            ++i;
        } else {
            long long null_region_size = 0;
            while (i < g_value_range_size && g_building_index->traces[i] == NULL) {
                --null_region_size;
                ++i;
            }
            DBGPRN("NULL region at %llu, size: %lli", packed_size * addr_size, null_region_size);
            memcpy(toc + packed_size * addr_size, (char*)&null_region_size, addr_size);
        }

    // convert TOC addresses to raw file offsets
    const u64 packed_toc_size = packed_size * addr_size;
    for (u64 i = 0; i < packed_size; ++i) {
        long long addr = 0;
        memcpy(&addr, toc + i * addr_size, addr_size);
        if ((addr >> (8 * addr_size - 1)) == 0) {
            addr += sizeof(struct index_t) + packed_toc_size;
            memcpy(toc + i * addr_size, &addr, addr_size);
        }
    }
    ret += write(fd, toc, packed_toc_size);
    // write content
    for (u64 i = 0; i < g_value_range_size; ++i)
        if (g_building_index->traces[i] != NULL) {
            const u64 len = g_building_index->traces[i]->size;
            ret += write(fd, &len, addr_size);
            ret += write(fd, &g_building_index->traces[i]->records, sizeof(struct record_t) * len);
        }

    free(toc);

    return ret;
}

static void index_deserialize()
{
    for (int i = 0; i < g_indexers_count; ++i) {
        if (g_index_arr[i] != NULL)
            continue;
            
        int pos = sprintf(g_inode_path, "%s", g_argv[g_dir_parms_pos + i]);
        if (g_inode_path[pos - 1] != '/') {
            g_inode_path[pos] = '/';
            ++pos;
        }
        strcpy(&g_inode_path[pos], INDEX_FILENAME);
        DBGPRN("Loading index file %s", g_inode_path);

        int fd = open(g_inode_path, O_RDONLY);
        if (fd <= 0) {
            ANSWER("Index file %s not found", g_inode_path);
            return;
        }
        struct stat fs;
        fstat(fd, &fs);

        char* buf = mmap(NULL, fs.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
        close(fd);
        if (buf == NULL)
            ERREXIT("Index file %s mapping failed", g_inode_path);

        u64 range = ((struct index_t*)buf)->range_size;
        const int index_size = sizeof(struct index_t) + range * sizeof(struct record_t*);
        struct index_t* index = malloc(index_size);
        if (index == NULL)
            ERREXIT("Allocating memory for reading index failed");
        g_index_arr[i] = index;
        memcpy(index, buf, sizeof(struct index_t));

        int addr_size = index->address_size;
        const int sign_shift = 8 * addr_size - 1;
        char* raw_toc = buf + sizeof(struct index_t);
        struct num_trace_t** traces = index->traces;
        DBGPRN("Reading %i index file, range=%llu", i, range);
        for (u64 j = 0, entry = 0; j < range; ++entry) {
            long long offset = 0;
            memcpy(&offset, raw_toc + entry * addr_size, addr_size);
            if (((offset >> sign_shift) & 1) == 0) {
                long long size = 0;
                memcpy(&size, buf + offset, addr_size);
                struct num_trace_t* records = malloc(sizeof(struct num_trace_t) + size * sizeof(struct record_t));
                if (records == NULL)
                    ERREXIT("Allocating memory for reading records failed");
                records->capacity = records->size = size;
                memcpy((char*)records + sizeof(struct num_trace_t), buf + offset + addr_size, size * sizeof(struct record_t));
                traces[j] = records;
                ++j;
            } else
                while (j < range && ((offset >> sign_shift) & 1) > 0) {
                    traces[j] = NULL;
                    ++offset;
                    ++j;
                }
        }
    }
}

static void* indexer_worker(void* serialize)
{
    g_indexer_processing = 1;
    gettimeofday(&g_last_indexing, NULL);
    // clear
    if (serialize != NULL)
        for (u64 i = 0; i < g_value_range_size; ++i)
            if (g_building_index->traces[i] != NULL)
                g_building_index->traces[i]->size = 0;
    // build index in memory
    struct dirent *dir;
    DIR* d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL && g_indexer_keep_alive == 1) {
            if (dir->d_type == DT_REG && dir->d_name[0] != '.')
                index_file(dir->d_name);
            else if (g_recursive && dir->d_type == DT_DIR)
                if (dir->d_name[0] != '.' && chdir(dir->d_name) == 0) {
                    indexer_worker(NULL);
                    if (chdir(".."))
                        ANSWER("Thats strange");
                }
        }
        closedir(d);
    }

    // store index to disk
    if (serialize != NULL && g_indexer_keep_alive == 1) {
        strcpy(g_index_path, TMP_FILENAME);
        int tmp_fd = mkstemp(g_index_path);
        size_t len = index_serialize(tmp_fd);
        unlink(g_index_path);

        off_t off = 0;
        int dst_fd = open(INDEX_FILENAME, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        sendfile(dst_fd, tmp_fd, &off, len);
        close(dst_fd);
        close(tmp_fd);
        // invalidate parent's cache
        union sigval sv;
        sv.sival_int = g_indexer_id;
        sigqueue(getppid(), SIGRESETCACHE, sv);

        g_indexer_processing = 0;
    }

    return serialize;
}

static void indexer_dispatcher(const int signal)
{
    char buf[32];
    struct timeval current_time;
    switch (signal) {
        case SIGINDEXINGSTART:
            pthread_create(&g_indexer_worker, NULL, &indexer_worker, (void*)&signal);
            break;
        case SIGINDEXINGSTATUS:
            if (g_indexer_processing) {
                gettimeofday(&current_time, NULL);
                sprintf(buf, "active for %li sec.", current_time.tv_sec - g_last_indexing.tv_sec);
            } else
                strcpy(buf, "inactive");
            ANSWER("Indexer on %s is %s", g_current_dir, buf);
            break;
        case SIGINT:
        case SIGTSTP:
            DBGPRN("Received stopping signal %i", signal);
            g_indexer_keep_alive = 0;
            break;
    }
}

static void indexer_main(const int indexer_id)
{
    g_indexer_id = indexer_id;
    g_current_dir = g_argv[g_dir_parms_pos + indexer_id];
    if (chdir(g_current_dir) != 0)
        ERREXIT("Opening directory %s failed", g_current_dir);

    pid_t old_worker;
    struct pidfh* pfh = pidfile_open(PID_FILENAME, 0600, &old_worker);
    if (pfh == NULL) {
        if (errno == EEXIST) 
            ERREXIT("Indexer is already running in %s, pid=%i", g_current_dir, old_worker);
        else
            ERREXIT("Creating lock file in %s failed", g_current_dir);
    }
    pidfile_write(pfh);

    signal(SIGINDEXINGSTART, indexer_dispatcher);
    signal(SIGINDEXINGSTATUS, indexer_dispatcher);
    signal(SIGTSTP, indexer_dispatcher);
    signal(SIGINT, indexer_dispatcher);

    g_number_min_len = log10(g_min_value) + 1;
    g_number_max_len = log10(g_max_value - 1) + 1;
    const int init_size = sizeof(struct index_t) + g_value_range_size * sizeof(struct record_t*);
    g_building_index = calloc(init_size, 1);
    if (g_building_index == NULL)
        ERREXIT("Allocating memory for the new index failed");
    g_building_index->min = g_min_value;
    g_building_index->range_size = g_value_range_size;

    if (access(INDEX_FILENAME, F_OK))
        indexer_dispatcher(SIGINDEXINGSTART);
    else
        gettimeofday(&g_last_indexing, NULL);

    while (g_indexer_keep_alive) {
        sleep(1);
        if (g_interval > 0) {
            struct timeval current_time;
            gettimeofday(&current_time, NULL);
            if (current_time.tv_sec - g_last_indexing.tv_sec >= (long int)g_interval)
                indexer_dispatcher(g_indexer_processing == 0 ? SIGINDEXINGSTART  : SIGINDEXINGSTATUS);

        }
    }

    if (g_indexer_processing != 0) {
        void* res;
        pthread_join(g_indexer_worker, &res);
    }

    for (u64 i = 0; i < g_value_range_size; ++i)
        free(g_building_index->traces[i]);
    free(g_building_index);
    pidfile_remove(pfh);
}

static char* get_inode_path(const char* dir, const ino_t inode)
{
    sprintf(g_inode_path, "find %s -xdev -inum %lu", dir, inode);
    FILE* fp = popen(g_inode_path, "r");
    if (fp == NULL)
        ERREXIT("get_inode_path: popen failed for %s %lu", dir, inode);

    char* ret = fgets(g_inode_path, PATH_MAX, fp);
    pclose(fp);

    if (ret == NULL) {
        if (errno == EINTR || g_fake_eof == 1) {
            g_fake_eof = 0;
            return get_inode_path(dir, inode);
        } else
            ERREXIT("get_inode_path: fgets failed for %s %lu", dir, inode);
    }

    const size_t len = strlen(ret);
    if (len > 0) ret[len - 1] = '\0';

    return ret;
}

static void read_parameter(const char* key, u64* value, const int has_value)
{
    int pos = 1;
    while (g_argv[pos] != NULL && strcmp(g_argv[pos], key) != 0)
        ++pos;

    if (g_argv[pos] != NULL) {
        if (has_value && g_argv[pos + 1] != NULL) {
            *value = atoll(g_argv[pos + 1]);
            g_dir_parms_pos += 2;
        } else {
            *value = 1;
            ++g_dir_parms_pos;
        }
    }
}

static void numf_signals(__attribute__ ((unused)) int sig, siginfo_t* info, __attribute__ ((unused)) void *ucontext)
{
    g_fake_eof = 1;
    const int data = info->si_value.sival_int;
    if (g_query_processing == 0 && g_index_arr[data] != NULL) {
        DBGPRN("Reset cached index %i in main process", data);
        for (u64 j = 0; j < g_index_arr[data]->range_size; ++j)
            free(g_index_arr[data]->traces[j]);
        free(g_index_arr[data]);
        g_index_arr[data] = NULL;
    }
}

static void query(const u64 n)
{
    int found = 0;
    g_query_processing = 1;
    ANSWER("Number %llu occurrences:", n);
    for (int i = 0; i < g_indexers_count; ++i) {
        struct index_t* index = g_index_arr[i];
        if (index == NULL) continue;
        const long long pos_in_range = n - index->min;
        if (pos_in_range < 0 || index->range_size <= (u64)pos_in_range) continue;
        struct num_trace_t* n_trace = index->traces[pos_in_range];
        if (n_trace != NULL) {
            found = 1;
            for (u64 j = 0; j < n_trace->size; ++j) {
                const char* path = get_inode_path(g_argv[g_dir_parms_pos + i], n_trace->records[j].file);
                ANSWER("%s:%llu", path, n_trace->records[j].position);
            }
        }
    }

    g_query_processing = 0;
    if (found == 0)
        ANSWER("not found");
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("Usage: %s [-r] [-m min=1] [-M max=1000] [-i interval=600] dir1 [dir2...]\n", argv[0]);
        return 1;
    }

    g_argv = argv;
    read_parameter("-r", &g_recursive, 0);
    read_parameter("-m", &g_min_value, 1);
    read_parameter("-M", &g_max_value, 1);
    read_parameter("-i", &g_interval, 1);
    assert(g_min_value <= g_max_value);

    g_indexers_count = argc - g_dir_parms_pos;
    g_value_range_size = g_max_value - g_min_value;
    g_indexers = (pid_t*)malloc(g_indexers_count * sizeof(pid_t));
    g_index_arr = (struct index_t**)calloc(g_indexers_count * sizeof(struct index_t*), 1);

    for (int i = 0; i < g_indexers_count; ++i) {
        if ((g_indexers[i] = fork()) == 0) {
            indexer_main(i);
            return 0;
        } else if (g_indexers[i] == -1)
            ERREXIT("Fork failed");
        else
            ANSWER("Starting indexer %i (pid=%i) for directory %s", i, g_indexers[i], argv[g_dir_parms_pos + i]);
    }
    assert(g_indexers_count == argc - g_dir_parms_pos);

    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = numf_signals;
    sigaction(SIGRESETCACHE, &act, NULL);

    while (1) {
        printf(PROMPT);
        char buf[MAX_CONSOLE_LINE + 1];
        int pos = 0;
        while (pos < MAX_CONSOLE_LINE) {
            const int c = getchar();
            if (c == '\n') break;
            if (c == EOF) {
                if (g_fake_eof == 1)
                    g_fake_eof = 0;
                else
                    goto bye;
            } else
                buf[pos++] = (char)c;
        }
        buf[pos] = '\0';

        if (!strcmp(buf, "status")) {
            DBGPRN("Querying indexers status");
            for (int i = 0; i < g_indexers_count; ++i)
                kill(g_indexers[i], SIGINDEXINGSTATUS);
            sleep(1);
        } else if (!strcmp(buf, "index")) {
            DBGPRN("Starting indexers..");
            for (int i = 0; i < g_indexers_count; ++i)
                kill(g_indexers[i], SIGINDEXINGSTART);
            sleep(1);
        } else if (!strncmp(buf, "query", 5)) {
            index_deserialize();
            char* p = buf + 5;
            while (1) {
                if (*p == '\0') break;
                while (*p != '\0' && !isdigit(*p)) ++p;
                const u64 n = atoll(p);
                query(n);
                while (*p != '\0' && isdigit(*p)) ++p;
            }
        } else if (!strcmp(buf, "exit")) {
bye:        for (int i = 0; i < g_indexers_count; ++i)
                kill(g_indexers[i], SIGTSTP);
            break;
        } else if (buf[0] != '\0')
            ANSWER("Valid commands: [status | index | query | exit]");
    }

    for (int i = 0; i < g_indexers_count; ++i) {
        waitpid(g_indexers[i], &argc, 0);
        if (g_index_arr[i] != NULL) {
            for (u64 j = 0; j < g_index_arr[i]->range_size; ++j)
                free(g_index_arr[i]->traces[j]);
            free(g_index_arr[i]);
        }
    }
    free(g_index_arr);
    free(g_indexers);
}

/*

Prerequisites:
sudo apt-get install libbsd-dev

Compile:
clear && gcc -Wall -Wextra -ggdb3 -Og -fsanitize=address -pthread numf.c -o numf -lbsd -lm
gcc -Wall -Wextra -O3 -pthread numf.c -o numf -lbsd -lm

Test run:
./numf -r -m 301 -M 981 -i 240 /tmp/numf_dir1 /tmp/numf_dir2

Clean:
hexdump -C .numf_index && rm .numf_index
for pid in `ps aux | grep numf | grep -v grep | awk '{print $2}'`; do kill -SIGINT $pid; done

Featured features:
- using pidfile for file based exclusive lock
- using inodes to identify pathnames
- using per indexer caching in main process
- using dynamic address size in index file

*/
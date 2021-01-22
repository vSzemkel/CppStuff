
#define u64 unsigned long long
#define PID_FILENAME ".numf_pid"
#define INDEX_FILENAME ".numf_index"
#define TMP_FILENAME "/tmp/numf-tmpXXXXXX"
#define CLI_HISTORY_LEN 20
#define MAX_CONSOLE_LINE 1000
#define SIGINDEXINGSTART SIGUSR1
#define SIGINDEXINGSTATUS SIGUSR2
#define SIGRESETCACHE SIGUSR1

#define DEBUGDIAG 0
#define PROMPT "numf> "
#define ANSWER(s, ...) printw(s"\n" __VA_OPT__(,) __VA_ARGS__); refresh()
#define DBGPRN(s, ...) if (DEBUGDIAG) { printf("diag> [%i:%i] "s"\n", getpid(), gettid() __VA_OPT__(,) __VA_ARGS__); }
#define ERREXIT(s, ...) do { int err = errno; fprintf(stderr, s", %s (errno=%i)\n" __VA_OPT__(,) __VA_ARGS__ , strerror(err), err); exit(err); } while (0)

pid_t gettid();                     // declaration to silence warning

struct record_t
{
    ino_t file;                     // unique inode for <directory, path>
    u64 position;                   // offset in file
};

struct num_trace_t
{
    u64 capacity;                   // capacity allocated
    u64 size;                       // capacity in use
    struct record_t records[0];     // records right after the trace in memory layout
};

struct index_t
{
    u64 min;                        // minimal number to search for
    u64 range_size;                 // number of numbers to search for
    u64 records_count;              // total of numbers occurences
    union {
        u64 traces_count;           // number of numbers found
        u64 address_size;           // computed internal index address size
    };
    struct num_trace_t* traces[0];  // range_size slots for number traces
};

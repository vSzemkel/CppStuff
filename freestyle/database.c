#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_LENGTH 32

// Simple in memory database in pure C

struct vote_t {
    short len;
    char option[MAX_LENGTH];
    unsigned short count;
};

struct database_t {
    short locked;
    short size;
    struct vote_t votes[];
};

int g_database_bytesize = 0;
struct database_t* g_database = NULL;

void save(const char* filename)
{
    FILE* fd = fopen(filename, "w+");
    if (fd == NULL) {
        printf("Can't create file %s\n", filename);
        exit(1);
    }

    fprintf(fd, "%d %d\n", g_database->locked, g_database->size);
    for (int i = 0; i < g_database->size; ++i) {
        struct vote_t* vote = &g_database->votes[i];
        fprintf(fd, "%hd ", vote->len);
        fwrite(vote->option, vote->len, 1, fd);
        fprintf(fd, " %d\n", vote->count);
    }

    fclose(fd);
    exit(0);
}

void initmem(const short locked, const short size)
{
    g_database_bytesize = sizeof(struct database_t) + size * sizeof(struct vote_t);
    g_database = malloc(g_database_bytesize);
    g_database->locked = locked;
    g_database->size = size;
}

void load(const char* filename)
{
    FILE* fd = fopen(filename, "r");
    if (fd == NULL) {
        printf("Can't open file %s\n", filename);
        exit(1);
    }

    short l, s;
    fscanf(fd, "%hd %hd\n", &l, &s);
    initmem(l, s);
    for (int i = 0; i < g_database->size; ++i) {
        struct vote_t* vote = &g_database->votes[i];
        fscanf(fd, "%hd ", &vote->len);
        fread(vote->option, vote->len, 1, fd);
        fscanf(fd, " %hd\n", &vote->count);
    }

    fclose(fd);
}

void create(const int argc, char** const argv)
{
    short argp = 2;
    short locked = 0;
    if (strcmp(argv[argp], "-l") == 0) {
        locked = 1;
        ++argp;
    }

    const char* filename = argv[argp++];
    short size = argc - argp;

    initmem(locked, size);
    for (int i = 0; i < size; ++i) {
        int len = strlen(argv[argp + i]);
        if (len > MAX_LENGTH) len = MAX_LENGTH;
        struct vote_t* vote = &g_database->votes[i];
        strncpy(vote->option, argv[argp + i], len);
        vote->len = len;
        vote->count = 0;
    }

    save(filename);
}

void show(const int argc, char** const argv)
{
    load(argv[2]);

    printf("Database filename: %s\n", argv[2]);
    for (int i = 0; i < g_database->size; ++i)
        printf("%-*s %d\n", MAX_LENGTH, g_database->votes[i].option, g_database->votes[i].count);
    printf("Database is %s\n", g_database->locked == 0 ? "unlocked" : "locked");
}

void vote(const int argc, char** const argv)
{
    short argp = 2;
    short decr = 0;
    if (strcmp(argv[argp], "-r") == 0) {
        decr = 1;
        ++argp;
    }

    const char* filename = argv[argp++];
    load(filename);

    short found = 0;
    for (int i = 0; i < g_database->size; ++i) {
        struct vote_t* vote = &g_database->votes[i];
        if (strncmp(vote->option, argv[argp], MAX_LENGTH) == 0) {
            if (decr) {
                if (vote->count > 0)
                    --vote->count;
            } else
                ++vote->count;

            found = 1;
            break;
        }
    }

    if (found == 0) {
        if (g_database->locked != 0)
            puts("Can't extend locked database\n");
        else {
            // realloc
            g_database_bytesize += sizeof(struct vote_t);
            g_database = realloc(g_database, g_database_bytesize);
            // add
            int len = strlen(argv[argp]);
            if (len > MAX_LENGTH) len = MAX_LENGTH;
            struct vote_t* vote = &g_database->votes[g_database->size];
            strncpy(vote->option, argv[argp], len);
            vote->count = 1;
            vote->len = len;
            ++g_database->size;
        }
    }

    save(filename);
}

int main(int argc, char** argv)
{
    if (argc < 3) {
        printf("Usage: %s [create | show | vote] options\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "create") == 0) create(argc, argv);
    if (strcmp(argv[1], "show") == 0) show(argc, argv);
    if (strcmp(argv[1], "vote") == 0) vote(argc, argv);
}

/*

gcc -Wall -g database.c -o database.o

./database.o create data.vdb "Option1" "Option   2" "Option3"
./database.o show data.vdb
./database.o vote data.vdb "Option1"

*/
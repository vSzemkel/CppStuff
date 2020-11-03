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
        fprintf(fd, "%hd ", g_database->votes[i].len);
        fwrite(g_database->votes[i].option, g_database->votes[i].len, 1, fd);
        fprintf(fd, " %d\n", g_database->votes[i].count);
    }

    fclose(fd);
    exit(0);
}

void initmem(const short locked, const short size)
{
    const int bytes = 2 * sizeof(short) + size * sizeof(struct vote_t);
    g_database = malloc(bytes);
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
        fscanf(fd, "%hd ", &g_database->votes[i].len);
        fread(g_database->votes[i].option, g_database->votes[i].len, 1, fd);
        fscanf(fd, " %hd\n", &g_database->votes[i].count);
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
        strncpy(g_database->votes[i].option, argv[argp + i], len);
        g_database->votes[i].len = len;
        g_database->votes[i].count = 0;
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
    for (int i = 0; i < g_database->size; ++i)
        if (strncmp(g_database->votes[i].option, argv[argp], g_database->votes[i].len) == 0) {
            if (decr) {
                if (g_database->votes[i].count > 0)
                    --g_database->votes[i].count;
            } else
                ++g_database->votes[i].count;

            found = 1;
            break;
        }

    if (found == 0 && g_database->locked == 0) {
        // realloc
        int old_size = 2 * sizeof(short) + g_database->size * sizeof(struct vote_t);
        g_database = realloc(g_database, old_size + sizeof(struct vote_t));
        // add
        int len = strlen(argv[argp]);
        if (len > MAX_LENGTH) len = MAX_LENGTH;
        strncpy(g_database->votes[g_database->size].option, argv[argp], len);
        g_database->votes[g_database->size].count = 1;
        g_database->votes[g_database->size].len = len;
        ++g_database->size;
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

cc -Walls -g database.c -o database.o

./database.o create data.vdb "Option1" "Option   2" "Option3"
./database.o show data.vdb
./database.o vote data.vdb "Option1"

*/
#include "common.h"

static void* shm_init()
{
    int fd = shm_open(shm_path, O_RDWR, 0);
    if (fd == -1)
        ERREXIT("shm_open");

    void* buf = mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (buf == MAP_FAILED)
        ERREXIT("mmap");

    return buf;
}

static void shm_clear(void* buf)
{
    munmap(buf, shm_size);
    shm_unlink(shm_path);
}

static void consume(magazine_t& magazine)
{
    std::ifstream data("../test_input.dat");
    if (!data.is_open())
        ERREXIT("opening test_input.dat");

    int T; data >> T;
    char msgbuf[sizeof(msg_t)];
    auto msg = (msg_t*)msgbuf;
    for (int i = 0; i < T; ++i) {
        int kind, value; 
        std::string text;
        data >> kind >> value >> text;
        magazine.get(msgbuf);
        assert(strcmp(msg->text, text.c_str()) == 0);
        printf("Message [%i]: %i %s %i\n", i, msg->size, msg->text, msg->kind == msg_type_t::msg1 ? msg->u8 : msg->u32);
        data.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int main(int, char**)
{
    auto buf = shm_init();
    auto mag = new (buf) magazine_t(false);
    DBGPRN("Shared memory buffer of size %lu initialized at local address %p", mag->size(), buf);

    consume(*mag);

    shm_clear(buf);
}

// g++ -Wall -Wextra -ggdb3 -std=c++17 -fsanitize=address consumer.cpp -lrt -o shm_consumer
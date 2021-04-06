#include "common.h"

static void* shm_init()
{
    int fd = shm_open(shm_path, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
        ERREXIT("shm_open");

    if (ftruncate(fd, shm_size) == -1)
        ERREXIT("ftruncate");

    void* buf = mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (buf == MAP_FAILED)
        ERREXIT("mmap");

    return buf;
}

static void shm_clear(void* buf)
{
    munmap(buf, shm_size);
}

static void produce(magazine_t& magazine)
{
    std::ifstream data("../test_input.dat");
    if (!data.is_open())
        ERREXIT("opening test_input.dat");

    int T; data >> T;
    for (int i = 0; i < T; ++i) {
        int kind, value; 
        std::string text;
        data >> kind >> value >> text;
        const auto mk = kind == 1 ? msg_type_t::msg1 : msg_type_t::msg2;

        msg_t msg{mk, value, text.c_str()};
        magazine.put(std::move(msg));
        DBGPRN("Message [%i]: %s of size %i", i, msg.text, msg.size);

        data.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int main(int, char**)
{
    auto buf = shm_init();
    auto mag = new (buf) magazine_t(true);
    DBGPRN("Shared memory buffer of size %lu initialized at local address %p", mag->size(), buf);

    produce(*mag);

    shm_clear(buf);
}

// g++ -Wall -Wextra -ggdb3 -std=c++17 -fsanitize=address producer.cpp -lrt -o shm_producer
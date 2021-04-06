
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#include <string.h>

#include <atomic>
#include <array>
#include <cassert>
#include <chrono>
#include <fstream>
#include <limits>
#include <numeric>
#include <string>
#include <thread>

#define DEBUGDIAG 1
#define DBGPRN(s, ...) if (DEBUGDIAG) { printf("diag> [%i:%i] " s "\n", getpid(), gettid() __VA_OPT__(,) __VA_ARGS__); fflush(stdout); }
#define ERREXIT(s, ...) do { int err = errno; fprintf(stderr, s", %s (errno=%i)\n" __VA_OPT__(,) __VA_ARGS__ , strerror(err), err); exit(err); } while (0)

enum class msg_type_t {
    msg1,
    msg2
};

constexpr int max_length = 2048;
constexpr int shm_size = (DEBUGDIAG ? 50 : (1 << 20)); // 1MB for prod, small for debug edge cases
constexpr char shm_path[] = "/shm_amarula";
constexpr size_t size_overflow = sizeof(int); // in case size field would overlap in cyclic buffer

struct msg_t {
    msg_t(msg_type_t type, int value, const char* msg) : kind(type), u32(value) {
        const int msglen = strlen(msg) + 1;
        size = sizeof(msg_t) - max_length + msglen;
        memcpy(text, msg, msglen);
    }

    int size; // size of msg_t header and text message in consecutive memory
    msg_type_t kind;
    union {
        uint8_t u8;
        uint32_t u32;
    };
    char text[max_length];
};

/**
 * @brief Non allocating cyclic buffer
 * @param T T is item type that has size member, more usefull in uniform items scenarios
 * @param N STorage size in bytes
 * Magazine can store items of arbitrary size.
 * When it's empty, get operation blocks
 * When there is no storage for next item to store, put operation blocks
 * The intended usage is by placement new on shared memory block
 */
template <class T, size_t N>
class Magazine
{
  public:
    /**
    * @param producer Boolean flag to differentiate producer from consumer
    * as consumer must not initialize Magazine state
    */
    Magazine(bool producer = false) {
        if (!producer) return;
        _start.store(0);
        _size.store(0);
    }

    size_t size() const {
        return N;
    }

    void put(T&& item) {
        const auto size = item.size;
        const auto high_water_mark = N - size;
        while (_size > high_water_mark)
            std::this_thread::sleep_for(std::chrono::seconds(1));

        const auto buf = (const char*)&item;
        const auto end = (_start + _size) % N;
        if (end + size <= N)
            memcpy(_storage + end, buf, size);
        else { // overlapping
            const auto tail = end + size - N;
            const auto head = size - tail;
            DBGPRN("Fragmented %lu:%lu", head, tail);
            memcpy(_storage + end, buf, std::max(head, size_overflow));
            memcpy(_storage, buf + head, tail);
        }

        _size += size;
    }

    void get(char* buf) {
        while (_size < sizeof(int))
            std::this_thread::sleep_for(std::chrono::seconds(1));

        const auto size = *((int*)&_storage[_start]);
        if (_start + size <= N)
            memcpy(buf, _storage + _start, size);
        else { // overlapping
            const auto tail = _start + size - N;
            const auto head = size - tail;
            memcpy(buf, _storage + _start, head);
            memcpy(buf + head, _storage, tail);
        }

        _size -= size;
        _start = (_start + size) % N;
    }

  private:
    std::atomic<size_t> _start;
    std::atomic<size_t> _size;
    char _storage[0];
};

using magazine_t = Magazine<msg_t, shm_size - sizeof(Magazine<msg_t, 0>) - size_overflow>;

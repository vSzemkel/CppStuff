
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include <atomic>
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

constexpr int port = 61337;
constexpr int max_length = 2048;
constexpr char server_name[] = "127.0.0.1";
constexpr int buf_size = (DEBUGDIAG ? 30 : (1 << 20)); // 1MB for prod, small for debug edge cases
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
 * @brief Allocating cyclic buffer with storage space of N bytes
 * @param T T is item type that has size member, more usefull in uniform items scenarios
 * @param N STorage size in bytes
 * Magazine can store items of arbitrary size.
 * When it's empty, get operation blocks
 * When there is no storage for next item to store, put operation blocks
 */
template <class T, size_t N>
class Magazine
{
  public:
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
    std::atomic<size_t> _start{0};
    std::atomic<size_t> _size{0};
    char _storage[N + size_overflow];
};

using magazine_t = Magazine<msg_t, buf_size>;

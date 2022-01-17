#include <array>

struct game_t : std::array<char, 8> {
    game_t(const int64_t val = 0) { *(int64_t*)data() = val; }
    void operator=(const int64_t val) { *(int64_t*)data() = val; }
    operator int64_t() const { return *(int64_t*)data(); }
};

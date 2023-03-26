// for general DP approach, see: \kickstart\2022\interesting_integers.cpp

constexpr const int LEN = 19;
int64_t factor10[LEN];

// precompute factor10 up to the longest number
factor10[0] = 1;
for (int i = 1; i < LEN; ++i)
    factor10[i] = 10 * factor10[i - 1];

assert(0 <= n && n < std::floor(log10(n)) + 1);
nth_digit = d / factor10[n] % 10 // n == 0 for less significant

auto nth = [](const int n, const int pos) { return n / factor10[pos] % 10; };
assert(nth(123456, 2) == 4);

// number of digits for non negative signed integral, assert(numlen(n) == 1 + (int)std::log10(n))
static int numlen(const uint64_t n) {
    int len{0}; 
    uint64_t probe{1};
    while (probe <= n) {
        probe *= 10;
        ++len;
    }
    return len;
}

constexpr const int LETTERS = 26;
using freq_t = std::array<int, LETTERS>;

static freq_t scan(std::string_view sv) {
    freq_t ret{};
    for (const char c : sv)
        ++ret[c - 'a'];
    return ret;
}
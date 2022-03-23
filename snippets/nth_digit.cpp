// for general DP approach, see: \kickstart\2022\interesting_integers.cpp

constexpr const int LEN = 19;
std::array<int64_t, LEN> factor10;

// precompute factor10 up to the longest number
for (int i = 0; i < LEN; ++i) {
    factor10[i] = (i == 0) ? 1 : 10 * factor10[i - 1];

assert(0 <= n && n < std::floor(log10(n)) + 1);
nth_digit = d / factor10[n] % 10 // n == 0 for less significant

auto nth = [](const int n, const int pos) { return n / factor10[pos] % 10; };
assert(nth(123456, 2) == 4);

// number of digits for non negative integral
static int numlen(const int64_t n) {
    int len{0}; 
    int64_t probe{1};
    while (probe <= n) {
        probe *= 10;
        ++len;
    }
    return len;
}

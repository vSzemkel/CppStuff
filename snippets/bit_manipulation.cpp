#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>  // Not just <immintrin.h> for compilers other than icc
#endif

// for bits higher then 32
int64_t two42 = int64_t{1} << 42
// value of the lowest set bit
n & -n
// turn off lowest set bit
n = n & (n - 1)
// check if n is a power of 2
((n & (n - 1)) == 0)
// get 2**lowest set bit
l = n - n & (n - 1)
// lowercase/uppercase 'A'==65, 'a'=97
bool is_upper = letter < 'a';
letter |= 0x20;  // to_lower
letter &= ~0x20; // to_upper
// ascii digits
char to_char = '0' + 5;
int to_number = to_char & 0x0f; // to_number == 5
// from Benq (GNU)
constexpr int pct(int x) { return __builtin_popcount(x); } // # of bits set
constexpr int bits(int x) { return x == 0 ? 0 : 31 - __builtin_clz(x); } // floor(log2(x)) 
// rotate N bits
static int rot(const int v, const int r) { // rotl iff r>0
    const auto s = ((r % N) + N) % N;
    if (!s) return v;
    return ((v << s) | (v >> (N - s))) & ((1 << N) - 1);
}
// bit iteration
for (int nodes = flag; nodes; ) {
    const int n = nodes - (nodes & (nodes - 1)); // lowest set
    const int bit_no = bits(n);
    /* */
    nodes -= n;
}
// all subsets sums
const int MAX = 1 << N;
std::vector<int64_t> sums(MAX); // path_count of nodes subset
for (int m = 1, hi = 0; m < MAX; ++m) {
    const int pred = m & (m - 1); // m with lowest bit cleared
    if (pred == 0) {
        sums[m] = A[hi++];
        solutions[m] = 1;
    } else 
        sums[m] = sums[pred] + A[bits(m - pred)];
}

// how many subsets has a property
// check seeds
const auto MX = 1 << N;
std::vector<int64_t> memo(MX);
for (int m = 0; m < MX; ++m) {
    int64_t sum{};
    for (int i = 0; i < N; ++i)
        if (m & (1 << i))
            sum += B[i];
    if (H <= sum)
        absentB[m] = 1;
}
// count possibilities
for (int i = 0; i < N; ++i)
    for (int m = 0; m < MX; ++m)
        if ((m & (1 << i)) == 0)
            memo[m] += memo[m ^ (1 << i)];

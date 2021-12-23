// value of the lowest bit
n & -n
// turn off lowest bit
n = n & (n - 1)
// check if n is a power of 2
((n & (n - 1)) == 0)
// get 2**lowest bit
l = n - n & (n - 1)
// lowercase/uppercase 'A'==65, 'a'=97
bool is_upper = letter < 'a';
letter |= 0x20;  // to_lower
letter &= ~0x20; // to_upper
// from Benq (GNU)
constexpr int pct(int x) { return __builtin_popcount(x); } // # of bits set
constexpr int bits(int x) { return x == 0 ? 0 : 31 - __builtin_clz(x); } // floor(log2(x)) 
// bit iteration
for (int nodes = flag; nodes; ) {
    const int n = nodes - (nodes & (nodes - 1));
    const int bit_no = bits(n);
    all_adj |= edge_mask[n];
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

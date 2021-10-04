// turn off lowest bit
n = n & (n - 1)
// check if n is a power of 2
((n & (n - 1)) == 0)
// get 2**lowest bit
l = n - n & (n - 1)
// bit iteration
for (int nodes = flag; nodes; ) {
    const int n = nodes - (nodes & (nodes - 1));
    all_adj |= edge_mask[n];
    nodes -= n;
}
// from Benq (GNU)
constexpr int pct(int x) { return __builtin_popcount(x); } // # of bits set
constexpr int bits(int x) { return x == 0 ? 0 : 31-__builtin_clz(x); } // floor(log2(x)) 

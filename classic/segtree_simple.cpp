
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <vector>

// Find maximal element on arbitrary subrange

template <typename T = int64_t>
struct simple_segment_tree_t
{
  public:
    simple_segment_tree_t(const std::vector<T>& input) {
        const int input_size = (int)input.size();
        while (offset < input_size) offset *= 2;
        nodes.resize(2 * offset);
        std::copy(input.begin(), input.end(), nodes.begin() + offset);
        for (int n = offset - 1; n > 0; --n)
            nodes[n] = join(nodes[2 * n], nodes[2 * n + 1]);
    }

    T query(int lower, int upper) const { // range [lower, upper)
        T ret{};
        for (lower += offset, upper += offset; lower < upper; lower >>= 1, upper >>= 1) { 
            if (lower & 1)
                ret = join(ret, nodes[lower++]); 
            if (upper & 1)
                ret = join(ret, nodes[--upper]);
        }

        return ret;
    }

    void update(int pos, const T val) {
        pos += offset;
        nodes[pos] = val;
        while (pos > 1) {
            pos /= 2;
            nodes[pos] = join(nodes[2 * pos], nodes[2 * pos + 1]);
        }
    }

    static const int MIN32 = std::numeric_limits<int>::min();

  private:
    static T join(const T& lhs, const T& rhs) {
        return std::max(lhs, rhs); // TO DO: change join implementation
    }

    std::vector<T> nodes;
    int offset{1};
};

void fuzz()
{
    constexpr int max = 1000;
    constexpr int trials = 1000;
    constexpr int size = 763783;
    const auto rand_in_range = [](const int ubound){std::random_device seed;std::mt19937 gen{seed()};std::uniform_int_distribution<int> dist(0,ubound-1);return dist(gen);};

    std::vector<int> input(size);
    for (int i = 0; i < size; ++i)
        input[i] = rand_in_range(max);

    simple_segment_tree_t<int> mst{input};
    for (int t = 0; t < trials; ++t) {
        int lower = rand_in_range(size);
        int upper = rand_in_range(size);
        if (lower > upper) std::swap(lower, upper);

        int ret{simple_segment_tree_t<>::MIN32};
        for (int i = lower; i < upper; ++i)
            ret = std::max(ret, input[i]);
        assert(ret == mst.query(lower, upper));

        const int pos = rand_in_range(size);
        const int val = rand_in_range(max);
        mst.update(pos, val);
        for (int i = lower; i < upper; ++i)
            ret = std::max(ret, input[i]);
        assert(ret == mst.query(lower, upper));
    }
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    simple_segment_tree_t<int> mst = std::vector<int>{1, 15, 3, 7, 11};
    assert(mst.query(0, 5) == 15);
    assert(mst.query(1, 4) == 15);
    assert(mst.query(2, 5) == 11);
    assert(mst.query(4, 5) == 11);
    assert(mst.query(2, 4) == 7);
    assert(mst.query(2, 3) == 3);
    assert(mst.query(0, 1) == 1);

    fuzz();
    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 segtree_simple.cpp -o segtree_simple.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 segtree_simple.cpp -o segtree_simple.o

Run:
segtree_simple.exe

*/

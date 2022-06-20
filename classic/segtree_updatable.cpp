
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <vector>

// Find maximal element on arbitrary subrange
// see: /kickstart/2019/diverse_subarray.cpp

/**
 * @brief TO DO
 * - update join
 * - update SEED to be neutral element for join, do not consider seed (free) slots
 */

template <typename T = int64_t>
struct updatable_segment_tree_t
{
  public:
    updatable_segment_tree_t(const int size) : _size(size), _changes(size << 2), _nodes(size << 2) {
        _build(1, 0, _size);
    }

    T query(const int lower, const int upper) { // range [lower, upper)
        return _query(lower, upper, 1, 0, _size);
    }

    void update(const int lower, const int upper, const T delta) {
        _update(lower, upper, delta, 1, 0, _size);
    }

    static inline const T SEED{0}; // TO DO: change value accordingly
    static inline const T ZERO{0}; // TO DO: change value accordingly

  private:
    static T join(const T& lhs, const T& rhs) {
        return std::max(lhs, rhs); // TO DO: change join implementation accordingly
    }

    inline void _upd(const int id, const T delta) {
        _changes[id] += delta;
        _nodes[id] += delta;
    }

    inline void _push(const int id, const int left, const int right) {
        if (_changes[id] == ZERO)
            return;
        _upd(left, _changes[id]);
        _upd(right, _changes[id]);
        _changes[id] = ZERO;
    }

    void _build(const int root, const int left, const int right) {
        _changes[root] = ZERO;
        _nodes[root] = SEED;
        if (right - left < 2)
            return ;

        const int mid = left + ((right - left) >> 1), il = root << 1, ir = il + 1;
        _build(il, left, mid);
        _build(ir, mid, right);
    }

    T _query(const int lower, const int upper, const int root, const int l, const int r) { // range [lower, upper)
        if (lower >= r || upper <= l)
            return 0;
        if (lower <= l && r <= upper)
            return _nodes[root];

        const int mid = l + ((r - l) >> 1), il = root << 1, ir = il + 1;
        _push(root, il, ir);
        return join(_query(lower, upper, il, l, mid), _query(lower, upper, ir, mid, r));
    }

    void _update(const int lower, const int upper, const T delta, const int root, const int l, const int r) {
        if (lower >= r || upper <= l)
            return;
        if (lower <= l && r <= upper) {
            _upd(root, delta);
            return;
        }
        const int mid = l + ((r - l) >> 1), il = root << 1, ir = il | 1;
        _push(root, il, ir);
        _update(lower, upper, delta, il, l, mid);
        _update(lower, upper, delta, ir, mid, r);
        _nodes[root] = join(_nodes[il], _nodes[ir]);
    }

    int _size;
    std::vector<T> _changes, _nodes;
};

void fuzz()
{
    constexpr int max = 1000;
    constexpr int trials = 1000;
    constexpr int size = 763783;
    const auto rand_in_range = [](const int ubound){std::random_device seed;std::mt19937 gen{seed()};std::uniform_int_distribution<int> dist(0,ubound-1);return dist(gen);};

    std::vector<int> input(size);
    updatable_segment_tree_t<int> ust{size};
    for (int i = 0; i < size; ++i)
        ust.update(i, i + 1, input[i] = rand_in_range(max));

    for (int t = 0; t < trials; ++t) {
        int lower = rand_in_range(size);
        int upper = rand_in_range(size);
        if (lower > upper) std::swap(lower, upper);

        int ret{updatable_segment_tree_t<>::SEED};
        for (int i = lower; i < upper; ++i)
            ret = std::max(ret, input[i]);
        assert(ret == ust.query(lower, upper));

        int pos1 = rand_in_range(size);
        int pos2 = rand_in_range(size);
        if (pos2 < pos1)
            std::swap(pos1, pos2);
        const int val = rand_in_range(max);
        ust.update(pos1, pos2, val);
        for (int i = pos1; i < pos2; ++i)
            input[i] += val;
        for (int i = lower; i < upper; ++i)
            ret = std::max(ret, input[i]);
        assert(ret == ust.query(lower, upper));
    }
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    updatable_segment_tree_t<int> ust(5);
    
    // std::vector<int>{1, 15, 3, 7, 11};
    ust.update(0, 1, 1);
    ust.update(1, 2, 15);
    ust.update(2, 3, 3);
    ust.update(3, 4, 7);
    ust.update(4, 5, 11);
    
    assert(ust.query(0, 5) == 15);
    assert(ust.query(1, 4) == 15);
    assert(ust.query(2, 5) == 11);
    assert(ust.query(4, 5) == 11);
    assert(ust.query(2, 4) == 7);
    assert(ust.query(2, 3) == 3);
    assert(ust.query(0, 1) == 1);
    ust.update(1, 4, 2);
    assert(ust.query(0, 5) == 17);

    fuzz();
    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 segtree_updatable.cpp -o segtree_updatable.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 segtree_updatable.cpp -o segtree_updatable

Run:
segtree_updatable.exe

*/


#include <array>
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

// Given a range of values and set of subrange queries, find minimal element for each query
// Arpa's trick A.K.A offline minimum element problem
// https://cp-algorithms.com/data_structures/disjoint_set_union.html

constexpr const std::array data = {7, 3, 5, 1, 2, 7, 9, 2, 3, 5, 2, 1, 7, 2, 4, 6, 8, 3, 7, 1};

struct uf_t {
    uf_t(const int size) : _count(size), _groups(size), _sizes(size, 1) {
        std::iota(_groups.begin(), _groups.end(), 0);
    }

    int find(const int x) {
        auto& g = _groups[x];
        if (x == g) return x;
        g = find(g);
        return g;
    }

    bool connected(const int x, const int y) {
        return find(x) == find(y);
    }

    void unite(const int x, const int y) {
        int gx = find(x);
        int gy = find(y);
        if (gx == gy)
            return;
        if (data[gx] >= data[gy])
            std::swap(gx, gy);
        _sizes[gx] += _sizes[gy];
        _groups[gy] = gx;
        --_count;
    }

    int size(const int x) { return _sizes[find(x)]; }
    int count() const { return _count; }
  private:
    int _count;
    std::vector<int> _groups, _sizes;
};

struct query_t {
    int left, right; // defines range [left..right]
    int index;       // index in answer table
};

int main(int, char**)
{
    const auto size = int(data.size());
    std::vector<std::vector<query_t>> queries(size); // queries[r] hold list of queries with range end equal to r

    int index{0};
    queries[0].push_back({0, 0, index++}); // 7
    queries[2].push_back({0, 2, index++}); // 3
    queries[3].push_back({2, 3, index++}); // 1
    queries[6].push_back({4, 6, index++}); // 2
    queries[6].push_back({1, 6, index++}); // 1
    queries[9].push_back({4, 9, index++}); // 2
    queries[15].push_back({5, 15, index++}); // 1
    queries[19].push_back({0, 19, index++}); // 1

    uf_t uf{size};
    std::vector<int> stack, ans(index);
    for (int i = 0; i < size; ++i) {
        while (!stack.empty() && data[stack.back()] > data[i]) {
            uf.unite(stack.back(), i);
            stack.pop_back();
        }
        stack.push_back(i);
        for (const auto& q : queries[i]) {
            ans[q.index] = data[uf.find(q.left)];
        }
    }

    assert((ans == std::vector<int>{7, 3, 1, 2, 1, 2, 1, 1}));
    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 arpa_rmq.cpp -o arpa_rmq.exe

*/

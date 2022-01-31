
#include <assert.h>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

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
        if (gx == gy) return;
        if (_sizes[gx] < _sizes[gy]) std::swap(gx, gy); // could be tailored, see \classic\arpa_rmq.cpp
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

int main(int, char**)
{
    uf_t uf(10);
    assert(uf.find(5) == 5);
    assert(uf.count() == 10);
    assert(uf.size(5) == 1);
    uf.unite(5, 8);
    assert(uf.find(5) == 5);
    assert(uf.count() == 9);
    assert(uf.size(5) == 2);
    uf.unite(8, 2);
    assert(uf.find(5) == 5);
    assert(uf.count() == 8);
    assert(uf.size(5) == 3);
    assert(uf.connected(2, 5));
    uf.unite(1, 3);
    uf.unite(6, 1);
    uf.unite(9, 1);
    assert(uf.count() == 5);
    uf.unite(5, 1);
    assert(uf.find(5) == 1);
    assert(uf.count() == 4);
    assert(uf.find(2) == 1);
    uf.unite(5, 7);
    uf.unite(5, 4);
    uf.unite(0, 1);
    assert(uf.count() == 1);
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 union_find.cpp -o union_find.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address union_find.cpp -o union_find

*/

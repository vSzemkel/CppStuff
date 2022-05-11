
#include <assert.h>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

struct uf_t {
    uf_t(const int size) : _count(size), _group(size), _rank(size), _size(size, 1) {
        std::iota(_group.begin(), _group.end(), 0);
    }

    int find(const int x) {
        auto& g = _group[x];
        if (x == g) return x;
        g = find(g);
        return g;
    }

    bool connected(const int x, const int y) {
        return find(x) == find(y);
    }

    bool unite(const int x, const int y) {
        int gx = find(x);
        int gy = find(y);
        if (gx == gy)
            return false;
        if (_rank[gx] < _rank[gy]) // could be tailored, see \classic\arpa_rmq.cpp
            std::swap(gx, gy);
        else if (_rank[gx] == _rank[gy])
            ++_rank[gx];
        _size[gx] += _size[gy];
        _group[gy] = gx;
        --_count;
        return true;
    }

    int size(const int x) { return _size[find(x)]; }
    int count() const { return _count; }
  private:
    int _count;
    std::vector<int> _group, _rank, _size;
};

constexpr const int N = 10;

int main(int, char**)
{
    uf_t uf(N);
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
    assert(uf.find(5) == 5);
    assert(uf.count() == 4);
    assert(uf.find(2) == 5);
    uf.unite(5, 7);
    uf.unite(5, 4);
    assert(uf.unite(0, 1) == true);
    assert(uf.unite(0, 1) == false);
    assert(uf.count() == 1);
    for (int i = 0; i < N; ++i)
        assert(uf.find(i) == 5);
    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 union_find.cpp -o union_find.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address union_find.cpp -o union_find

*/

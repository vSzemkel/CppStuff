
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

// Cherries Mesh
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050edb/0000000000170721

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

static void solve() {
    int N, M;
    std::cin >> N >> M;
    uf_t uf(N);

    for (int i = 0; i < M; ++i) {
        int r, c;
        std::cin >> r >> c;
        uf.unite(--r, --c);
    }

    const auto regions = uf.count();
    // (N - regions) * 1 + (regions - 1) * 2;
    std::cout << N + regions - 2;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 cherries_mesh.cpp -o cherries_mesh.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address cherries_mesh.cpp -o cherries_mesh

Run:
cherries_mesh.exe < cherries_mesh.in

Input:

2
2 1
1 2
3 1
2 3

Output:

Case #1: 1
Case #2: 3

*/

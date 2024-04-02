/*
ID: marcin.8
LANG: C++
TASK: schlnet
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=cL0jbAFhIj3&S=schlnet
*/

#include <algorithm>
#include <cassert>
#include <fstream>
#include <numeric>
#include <unordered_map>
#include <utility>
#include <vector>

std::ifstream task_in("schlnet.in");
std::ofstream task_out("schlnet.out");

struct digraph_t
{
    digraph_t(const int size = 0) : _size(size), _order(size), _in(size), _out(size) {
    }

    void add_edge(const int from, const int to) {
        assert(0 <= from && from < _size);
        assert(0 <= to && to < _size);
        _out[from].push_back(to);
        _in[to].push_back(from);
    }

    auto get_sccomponents() {
        _order.clear();
        _used.assign(_size, false);
        for (int i = 0; i < _size; ++i) {
            if (!_used[i])
                first_dfs(i);
        }

        _comp.assign(_size, -1);
        for (int i = 0, ci = 0; i < _size; ++i) {
            const int v = _order[_size - i - 1];
            if (_comp[v] == -1)
                second_dfs(v, ci++);
        }

        return _comp;
    }

    auto scc_count() const {
        assert(int(_comp.size()) == _size);
        return 1 + *std::max_element(_comp.begin(), _comp.end());
    }

  private:
    int _size;
    std::vector<bool> _used;
    std::vector<int> _comp, _order;
    std::vector<std::vector<int>> _in, _out;

    void first_dfs(const int v) {
        _used[v] = true;
        for (int u : _out[v]) {
            if (!_used[u])
                first_dfs(u);
        }

        _order.push_back(v);
    }

    void second_dfs(const int v, const int ci) {
        _comp[v] = ci;
        for (int u : _out[v]) {
            if (_comp[u] == -1)
                second_dfs(u, ci);
        }
    }
};

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

int main(int, char**)
{
    int N;
    task_in >> N;
    uf_t uf{N};
    digraph_t dg{N};
    std::vector<bool> has_predecor(N), has_succesor(N);
    for (int s = 0; s < N; ++s)
        while (true) {
            int r;
            task_in >> r;
            if (--r < 0)
                break;
            if (s != r) {
                uf.unite(s, r);
                dg.add_edge(s, r);
                has_predecor[r] = true;
                has_succesor[s] = true;
            }
        }

    std::unordered_map<int, std::pair<int, int>> groups; // {group id, {starters count, terminals count}}
    for (int s = 0; s < N; ++s) {
        auto& g = groups[uf.find(s)];
        if (!has_succesor[s])
            ++g.second;
        if (!has_predecor[s])
            ++g.first;
    }

    const int parts_count = uf.count();

    // Subtask A: for every graph part provide software to its starters
    dg.get_sccomponents();
    const int starters = dg.scc_count();

    // Subtask B: minimal number of new connections to cover all from arbitrary school
    int connections = parts_count;
    if (connections == 1) 
        --connections;
    for (const auto& [g, t] : groups)
        if (uf.size(g) > 1) {
            connections += std::max(t.first, t.second);
            if (parts_count > 1 && t.second > 0)
                --connections;
        }

    task_out << starters << '\n' << connections << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 schlnet.cpp -o schlnet.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address schlnet.cpp -o schlnet

Input:

30
18 0
7 21 0
1 4 15 28 0
9 0
10 15 16 0
22 26 0
1 5 10 12 0
3 17 29 0
2 5 17 0
19 23 0
20 0
1 7 15 19 0
0
23 0
0
0
5 18 0
0
7 18 0
17 0
24 0
13 21 0
26 0
0
2 23 30 0
2 9 11 13 14 27 0
2 0
14 0
0
28 0

Output:

3
6

*/

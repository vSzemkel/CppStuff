
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <queue>
#include <vector>

// Directed graph
// Base operations implementation

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

    void analyze() {
        std::vector<int> in_degree(_size);
        std::transform(_in.begin(), _in.end(), in_degree.begin(), [](const auto& ie){ return ie.size(); });
        std::queue<int> q;

        int ord{0};
        for (int i = 0; i < _size; ++i)
            if (in_degree[i] == 0)
                q.push(i);
        while (!q.empty()) {
            const int n = q.front(); q.pop();
            _order[n] = ord++;
            for (const int next : _out[n]) {
                --in_degree[next];
                if (in_degree[next] == 0)
                    q.push(next);
            }
        }

        _has_cycle = ord < _size;
    }

    auto get_reversed() const {
        digraph_t ret{*this};
        std::swap(ret._in, ret._out);
        return ret;
    }

    auto has_cycle() const { return _has_cycle; }

    auto topological_order() const { return _has_cycle ? decltype(_order){} : _order; }

    auto get_sccomponents() {
        _order.clear();
        _used.assign(_size, false);
        for (int i = 0; i < _size; ++i) {
            if (!_used[i])
                first_dfs(i);
        }

        std::swap(_in, _out);
        _comp.assign(_size, -1);
        for (int i = 0, ci = 0; i < _size; ++i) {
            const int v = _order[_size - i - 1];
            if (_comp[v] == -1)
                second_dfs(v, ci++);
        }
        std::swap(_in, _out);

        return _comp;
    }

  private:
    int _size;
    bool _has_cycle;
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

int main(int, char**)
{
    digraph_t dg{6};
    dg.add_edge(0, 1);
    dg.add_edge(1, 2);
    dg.add_edge(1, 3);
    dg.add_edge(2, 4);
    dg.add_edge(3, 4);
    dg.add_edge(4, 5);

    dg.analyze();
    assert((dg.topological_order() == std::vector{0, 1, 2, 3, 4, 5}));
    auto rdg = dg.get_reversed();
    rdg.analyze();
    assert((rdg.topological_order() == std::vector{5, 4, 2, 3, 1, 0}));

    dg.add_edge(4, 1);
    dg.analyze();
    assert(dg.has_cycle());
    assert(dg.topological_order().empty());
    assert((dg.get_sccomponents() == std::vector{0, 1, 1, 1, 1, 2}));

    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 digraph.cpp -o digraph.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address digraph.cpp -o digraph

*/
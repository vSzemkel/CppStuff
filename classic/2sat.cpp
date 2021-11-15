
#include <array>
#include <assert.h>
#include <iostream>
#include <queue>
#include <vector>

// 2SAT solver
// Using digraph_t and Kosaraju's algorithm
// https://cp-algorithms.com/graph/2SAT.html

struct digraph_t
{
    digraph_t() : digraph_t(0) {}
    digraph_t(const int size) : _size(size), _order(size), _in(size), _out(size) {
    }

    digraph_t get_reversed() {
        digraph_t ret{*this};
        std::swap(ret._in, ret._out);
        return ret;
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

    auto has_cycle() const { return _has_cycle; }
    auto topological_order() const { return _has_cycle ? decltype(_order){} : _order; }

    friend struct twosat_t;

  private:
    int _size;
    bool _has_cycle;
    std::vector<int> _order;
    std::vector<std::vector<int>> _in, _out;
};

struct twosat_t
{
    using formula_t = std::vector<std::array<int, 2>>;

    twosat_t(formula_t formula) {
        _size = 0;
        for (const auto& c : formula)
            _size = std::max(_size, std::max(std::abs(c[0]), std::abs(c[1])));
        _size <<= 1;
        _dg = digraph_t(_size);
        for (const auto& c : formula) {
            _dg.add_edge(literal2node(-c[0]), literal2node(c[1]));
            _dg.add_edge(literal2node(-c[1]), literal2node(c[0]));
        }
        _rdg = _dg.get_reversed();
    }

    void first_dfs(const int v) {
        _used[v] = true;
        for (int u : _dg._out[v]) {
            if (!_used[u])
                first_dfs(u);
        }

        _order.push_back(v);
    }

    void second_dfs(const int v, const int ci) {
        _comp[v] = ci;
        for (int u : _rdg._out[v]) {
            if (_comp[u] == -1)
                second_dfs(u, ci);
        }
    }

    bool solve() {
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

        _solution.assign(_size / 2, false);
        for (int i = 0; i < _size; i += 2) {
            if (_comp[i] == _comp[i + 1])
                return false;
            _solution[i / 2] = _comp[i] > _comp[i + 1];
        }

        return true;
    }

    std::vector<bool> _solution;

  private:
    int _size; // two noder per variable
    digraph_t _dg, _rdg;
    std::vector<bool> _used;
    std::vector<int> _comp, _order;

    int literal2node(const int literal) { // [-_size / 2 .. _size / 2] -> [0.._size-1]
        if (literal > 0)
            return 2 * (literal - 1);
        return 2 * (-literal - 1) + 1;
    }
};

int main(int, char**)
{
    twosat_t::formula_t f {
        {1, -2},  // A || ~B
        {-1, 2},  // ~A || B
        {-1, -2}, // ~A || ~B
        {1, 3},   // A || C
    };

    twosat_t ts{f};
    assert(ts.solve());
    assert((ts._solution == std::vector<bool>{false, false, true}));

    twosat_t::formula_t f2 {
        {1, -2},  // A || ~B
        {-1, 2},  // ~A || B
        {-2, -1}, // ~B || ~A
        {1, 2},   // A || B
    };

    twosat_t ts2{f2};
    assert(ts2.solve() == false);
    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 2sat.cpp -o 2sat.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address 2sat.cpp -o 2sat

*/

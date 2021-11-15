
#include <assert.h>
#include <iostream>
#include <queue>
#include <vector>

// Directed graph
// Base operations implementation

struct digraph_t
{
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

  private:
    int _size;
    bool _has_cycle;
    std::vector<int> _order;
    std::vector<std::vector<int>> _in, _out;
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

    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 digraph.cpp -o digraph.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address digraph.cpp -o digraph

*/

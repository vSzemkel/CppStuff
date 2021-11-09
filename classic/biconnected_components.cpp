
#include <assert.h>
#include <iostream>
#include <utility>
#include <vector>

// Biconnected components
// Removing any of cut_nodes or cut_edges disconnects the graph
// Based on the book CP in Python 128, page 101

struct biconnected_components_t {
    biconnected_components_t(const int size) : _size(size), _adj(size), _num(size), _low(size, size), _parent(size, -1), _times_seen(size, -1), _crit_children(size) {
    }

    void add_edge(const int from, const int to) {
        assert(0 <= from && from < _size);
        assert(0 <= to && to < _size);
        _adj[from].push_back(to);
        _adj[to].push_back(from);
    }

    auto cut_nodes() const { return _cut_nodes; }
    auto cut_edges() const { return _cut_edges; }

    void search() {
        int time{0};
        std::vector<int> stack;
        for (int root = 0; root < _size; ++root)
            if (_times_seen[root] < 0) {
                _times_seen[root] = 0;
                stack.push_back(root);
                while (!stack.empty()) {
                    const int n = stack.back();
                    if (_times_seen[n] == 0)
                        _num[n] = time++;
                    if (_times_seen[n] == int(_adj[n].size())) { // processing of n finished
                        stack.pop_back();
                        const int up = _parent[n];
                        if (up >= 0) {
                            _low[up] = std::min(_low[up], _low[n]);
                            if (_low[n] >= _num[up])
                                ++_crit_children[up];
                        }
                    } else {
                        const int next = _adj[n][_times_seen[n]];
                        ++_times_seen[n];
                        if (_times_seen[next] < 0) {
                            _parent[next] = n;
                            _times_seen[next] = 0;
                            stack.push_back(next);
                        } else if (_num[next] < _num[n] && _parent[n] != next) // back arrow
                            _low[n] = std::min(_low[n], _num[next]);
                    }
                }
            }

        _cut_edges.clear();
        _cut_nodes.clear();
        for (int n = 0; n < _size; ++n)
            if (_parent[n] < 0) {
                if (_crit_children[n] >= 2)
                    _cut_nodes.push_back(n);
            } else {
                if (_crit_children[n] >= 1)
                    _cut_nodes.push_back(n);
                if (_low[n] >= _num[n])
                    _cut_edges.emplace_back(_parent[n], n);
            }
    }

  private:
    int _size;
    std::vector<int> _cut_nodes;
    std::vector<std::vector<int>> _adj;
    std::vector<std::pair<int, int>> _cut_edges;
    std::vector<int> _num, _low, _parent, _times_seen;
    std::vector<int> _crit_children; // cc[u] = #{children v | low[v] >= num[u]}
};

int main(int, char**)
{
    biconnected_components_t graph{11};
    graph.add_edge(0, 1);
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    graph.add_edge(3, 0);
    graph.add_edge(1, 4);
    graph.add_edge(2, 4);
    graph.add_edge(4, 5);
    graph.add_edge(5, 6);
    graph.add_edge(5, 7);
    graph.add_edge(6, 7);
    graph.add_edge(7, 4);
    graph.add_edge(6, 8);
    graph.add_edge(8, 9);
    graph.add_edge(9, 10);
    graph.add_edge(10, 8);

    graph.search();
    auto e = graph.cut_edges();
    auto n = graph.cut_nodes();

    assert((n == decltype(n){4, 6, 8}));
    assert((e.size() == 1 && e[0] == std::pair{6, 8}));
    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 biconnected_components.cpp -o biconnected_components.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address biconnected_components.cpp -o biconnected_components

Run:
biconnected_components.exe < biconnected_components.in

*/

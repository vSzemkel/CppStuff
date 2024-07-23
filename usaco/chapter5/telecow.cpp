/*
ID: marcin.8
LANG: C++
TASK: telecow
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=G0g2gioZqzP&S=telecow
*/

#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>

std::ifstream task_in("telecow.in");
std::ofstream task_out("telecow.out");

template <typename T = int>
struct vertex_flow_graph_t
{
    vertex_flow_graph_t(const int size = 0) 
        : _size(size), _adj(size), _capacity(_size, std::vector<int>(_size))
    { 
        reset(); 
    }

    void add_edge(const int from, const int to, const int capacity = INF) {
        assert(0 <= from && from < _size && 0 <= to && to < _size);
        _adj[from].push_back(to);
        _adj[to].push_back(from);
        _capacity[from][to] = _capacity[to][from] = capacity;
    }

    void reset() {
        const auto size = _adj.size();
        _pred.assign(size, -1);
        _seen.assign(size, false);
    }

    /**
     * Checks if target is reachable from suorce
     * Side effects: saves paths in _prev
     */
    bool bfs(const int source, const int target) {
        assert(0 <= source && source < _size && 0 <= target && target < _size);
        reset();
        std::queue<int> qq;
        qq.push(source);
        _pred[source] = -1;
        _seen[source] = true;
        while (!qq.empty()) {
            const int node = qq.front(); qq.pop();
            if (node == target)
                return true;
            for (const auto& next : _adj[node]) {
                if (!_seen[next] && _capacity[node][next] > 0) {
                    _seen[next] = true;
                    _pred[next] = node;
                    qq.push(next);
                }
            }
        }

        return false;
    }

    /**
     * Split nodes construction:
     * Copy a graph aside. For each edge A->B in original one replace it by two edges
     * First edge is A->B`, where A is a node in original graph, and B` is a copy of B
     * Second edge B`->B returns back to original graph
     * If the returning edge has unit capacity, vertex flow properties can be computed
     */
    void convert_to_vertex_flow_graph(const int vertex_capacity = 1) {
        const int vf_size = _size << 1;
        std::vector<std::vector<int>> adj(vf_size);
        std::vector<std::vector<int>> capacity(vf_size, std::vector<int>(vf_size));
        for (int f = 0; f < _size; ++f) {
            adj[f + _size].push_back(f);
            capacity[f + _size][f] = vertex_capacity;
            for (const auto& t : _adj[f]) {
                adj[f].push_back(t + _size);
                adj[t].push_back(f + _size);
                capacity[f][t + _size] = _capacity[f][t];
                capacity[t][f + _size] = _capacity[t][f];
            }
        }

        _adj = std::move(adj);
        _backup = _capacity = std::move(capacity);
    }

    /**
     * Every proper path must include returning edge, so it has unit flow
     * Skip internal edge in target node
     */
    T compute_max_flow(const int source, const int target) {
        T flow{};
        while (bfs(source, target)) {
            ++flow;
            for (int cur = _pred[target], prev = _pred[cur]; ~prev; cur = prev, prev = _pred[prev]) {
                --_capacity[prev][cur];
                ++_capacity[cur][prev];
            }
        }

        return flow;
    }

    /**
     * After disabling any cut vertex, the flow is reduced
     */
    auto get_cut_vertices(const int source, const int target, int count) {
        std::vector<int> ret;
        for (int v = 0; v < _size; ++v)
            if (v != source && v != target && _capacity[v + _size][v] == 0) {
                _capacity = _backup;
                _capacity[v + _size][v] = 0;
                if (compute_max_flow(source, target) + 1 == count) {
                    _backup[v + _size][v] = 0;
                    ret.push_back(v + 1);
                    --count;
                }
            }

        return ret;
    }

    static constexpr const int INF = (1 << 30) - 1;

  private:
    int _size;
    std::vector<bool> _seen;
    std::vector<int> _pred;
    std::vector<std::vector<int>> _adj, _backup, _capacity;
};

template <typename C>
static void print(const C& v, std::ostream& task_out = std::cout)
{
    if (v.empty())
        return;
    char sep = ' ';
    auto lst = v.size();
    for (const auto& e : v) {
        if (--lst == 0) sep = '\n';
        task_out << e << sep;
    }
}

int main(int, char**)
{
    int N, M, c1, c2;
    task_in >> N >> M >> c1 >> c2;
    --c1; --c2;
    vertex_flow_graph_t<int> g{N};
    for (int z = M; z; --z) {
        int f, t;
        task_in >> f >> t;
        g.add_edge(--f, --t);
    }

    g.convert_to_vertex_flow_graph();
    const int count = g.compute_max_flow(c1, c2);
    task_out << count << '\n';
    if (count > 0)
        print(g.get_cut_vertices(c1, c2, count), task_out);
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++20 telecow.cpp -o telecow.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++20 -fsanitize=address telecow.cpp -o telecow

Run:
telecow.exe && type telecow.out

Input:

20 26 1 2
1 9
1 17
1 19
2 6
2 9
2 14
3 15
3 16
3 18
4 8
5 6
5 7
5 12
5 17
6 7
6 19
7 13
7 14
7 15
9 12
9 14
10 14
11 12
12 17
13 15
15 17

Output:

3
6 7 9

*/

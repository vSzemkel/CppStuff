
#include <cassert>
#include <vector>

// see: /usaco/chapter4/ditch.cpp
template <typename T = int>
struct flow_graph_t
{
    flow_graph_t(const int sz, const int src, const int snk) : _size(sz), _source(src), _sink(snk), _flow{0} {
        assert(0 <= src && src < sz && 0 <= snk && snk < sz && src != snk);
        _adj.resize(_size);
    }

    void clear_flow() {
        _flow = 0;
        for (auto& e : _edges)
            e.flow = 0;
    }

    int add(const int from, const int to, const T forward_cap, const T backward_cap = T{}) {
        assert(0 <= from && from < _size && 0 <= to && to < _size);
        const int id = (int)edges.size();
        _adj[from].push_back(id);
        _edges.push_back({from, to, forward_cap, 0});
        _adj[to].push_back(id + 1);
        _edges.push_back({to, from, backward_cap, 0});
        return id;
    }

    struct edge_t {
        int from;
        int to;
        T capacity;
        T flow;
    };

    static constexpr T eps = (T)1e-9;

    std::vector<std::vector<int>> _adj;
    std::vector<edge_t> _edges;
    int _size;
    int _source;
    int _sink;
    T _flow;
};

int main(int, char**)
{
    flow_graph_t fg(3, 0, 2);
    return 0;
}

/* Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 flow_graph.cpp -o flow_graph.exe
*/
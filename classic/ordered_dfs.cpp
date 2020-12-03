
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <iterator>
#include <vector>

// Treverse graph with DFS starting each enclave from minimal node,
// choosing minimal adjacent node first

using label_t = uint16_t;

struct vertex_t {
    std::vector<label_t> edges;
    label_t label;
    bool visited;
};

class graph_t {
  public:
    graph_t(uint32_t v, uint32_t e) : _v(v), _e(e) {};
    void load();
    void enumerate();
  private:
    void dfs();
    void add_edge(const label_t& from, const label_t& to);
    vertex_t& get_node(label_t label) { return _nodes[label - 1]; }
    uint32_t _v, _e;
    std::vector<vertex_t> _nodes;
    std::vector<label_t> _dfs_stack;
};

void graph_t::add_edge(const label_t& from, const label_t& to)
{
    assert(1 <= from && from <= _v && 1 <= to && to <= _v);

    int ind;
    auto& from_edges = get_node(from).edges;
    for (ind = from_edges.size(); ind > 0 && from_edges[ind - 1] < to; --ind);
    from_edges.insert(from_edges.begin() + ind, to);
    auto& to_edges = get_node(to).edges;
    for (ind = to_edges.size(); ind > 0 && to_edges[ind - 1] < from; --ind);
    to_edges.insert(to_edges.begin() + ind, from);
}

void graph_t::load()
{
    label_t label{1};
    _nodes.resize(_v);
    for (auto& n : _nodes)
        n.label = label++;
    for (int i = 0; i < _e; ++i) {
        label_t f, t;
        std::cin >> f >> t;
        add_edge(f, t);
    }
}

void graph_t::dfs()
{
    while (!_dfs_stack.empty()) {
        auto label = _dfs_stack.back();
        _dfs_stack.pop_back();
        auto& node = get_node(label);
        if (!node.visited) {
            node.visited = true;
            std::cout << label << " ";
            for (const auto e : node.edges)
                if (!get_node(e).visited)
                    _dfs_stack.push_back(e);
        }
    }
}

void graph_t::enumerate()
{
    while (true) {
        const auto enclave = std::find_if(_nodes.begin(), _nodes.end(), [](const vertex_t& v) { return v.visited == false; });
        if (enclave == _nodes.end()) break;
        _dfs_stack.push_back(enclave->label);
        dfs();
    }
}

int main(int argc, char* argv[])
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    uint32_t no_of_cases, v, e;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cin >> v >> e;
        graph_t graph{v, e};
        graph.load();
        std::cout << "Case #" << g << ": ";
        graph.enumerate();
        std::cout << "\n";
    }
}

/*
clang++.exe -Wall -ggdb3 -O0 -std=c++17 ordered_dfs.cpp -o ordered_dfs.exe
ordered_dfs.exe < ordered_dfs.in

Input:

2
7 5
5 3
3 1
3 6
2 4
2 1
8 7
1 2
1 5
6 1
8 3
1 3
3 7
1 4

Output:

Case #1: 1 2 4 3 5 6 7 
Case #2: 1 2 3 7 8 4 5 6 

*/

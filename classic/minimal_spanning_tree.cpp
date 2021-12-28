
#include <iostream>
#include <limits>
#include <queue>

// for a given graph with positive length edges find
// the minimal spanning tree

// for solution with coincidence matrix see: /usaco/chapter3/agrinet.cpp

// case definition
struct edge_t {
    char parent;
    char label;
    int weight;
    friend bool operator<(const edge_t& lhs, const edge_t& rhs) { return lhs.weight > rhs.weight; }
};

constexpr int g_vertex_count = 13;
using connection_t = std::pair<char, int>; // {label, weight}
using graph_t = std::vector<std::vector<connection_t>>; // {label, weight}

// globals
graph_t g_graph;
int g_weight{};
std::vector<std::vector<connection_t>> g_tree;
std::vector<bool> g_visited(g_vertex_count);

void add_edge(const char n1, const char n2, const int len)
{
    g_graph[n1 - 'A'].emplace_back(n2, len);
    g_graph[n2 - 'A'].emplace_back(n1, len);
}

void init_graph()
{
    g_tree.resize(g_vertex_count);
    g_graph.resize(g_vertex_count);
    add_edge('A', 'B', 3);
    add_edge('A', 'C', 1);
    add_edge('B', 'E', 2);
    add_edge('B', 'F', 1);
    add_edge('C', 'F', 2);
    add_edge('C', 'G', 1);
    add_edge('D', 'H', 1);
    add_edge('C', 'D', 2);
    add_edge('D', 'I', 2);
    add_edge('G', 'H', 3);
    add_edge('H', 'I', 1);
    add_edge('E', 'J', 2);
    add_edge('F', 'J', 3);
    add_edge('G', 'K', 1);
    add_edge('H', 'L', 1);
    add_edge('K', 'L', 4);
    add_edge('J', 'M', 1);
    add_edge('K', 'M', 2);
}

void prim(const char start)
{
    std::priority_queue<edge_t> q;
    q.push(edge_t{start, start, 0});
    while (!q.empty()) {
        const auto e = q.top(); q.pop();
        const int node = e.label - 'A';
        if (!g_visited[node]) {
            g_visited[node] = true;
            g_tree[e.parent - 'A'].emplace_back(e.label, e.weight);
            g_weight += e.weight;
            for (const auto& adj : g_graph[node])
                if (!g_visited[adj.first - 'A'])
                    q.push(edge_t{e.label, adj.first, adj.second});
        }
    }
}

int main(int argc, char* argv[])
{
    init_graph();
    prim('A');

    // present result
    if (std::any_of(g_visited.begin(), g_visited.end(), [](const auto v){ return !v; }))
        std::cout << "\nThe graph is not coherent\n";
    else {
        std::cout << "\nEdges of minimal spanning tree of a weight " << g_weight << ":\n";
        for (int p = 0; p < g_vertex_count; ++p)
            if (!g_tree[p].empty())
            for (const auto& e : g_tree[p])
                if ('A' + p != e.first)
                    std::cout << '\t' << (char)('A' + p) << '-' << e.first << ' ' << e.second << '\n';
    }
}

/*    clang++.exe -Wall -g -std=c++17 minimal_spanning_tree.cpp -o minimal_spanning_tree.exe

Output:

Edges of minimal spanning tree of a weight 16:
    A-C 1
    B-E 2
    C-G 1
    C-F 2
    C-D 2
    D-H 1
    F-B 1
    G-K 1
    H-I 1
    H-L 1
    K-M 2
    M-J 1

*/
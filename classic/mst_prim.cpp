
#include <iostream>
#include <limits>
#include <queue>

// for a given graph with positive length edges find
// the minimal spanning tree


// case definition
constexpr int g_vertex_count = 13;
using edge_t = std::pair<char, int>; 
using graph_t = std::vector<std::vector<edge_t>>;
using comp_t = bool(*)(const edge_t&, const edge_t&);
using queue_t = std::priority_queue<edge_t, std::vector<edge_t>, comp_t>;

// globals
graph_t g_graph;
int g_weight = 0;
std::vector<edge_t> g_tree;
std::vector<uint8_t> g_visited(g_vertex_count, 0);

void add_edge(const char n1, const char n2, const int len)
{
    g_graph[n1 - 'A'].emplace_back(n2, len);
    g_graph[n2 - 'A'].emplace_back(n1, len);
}

void init_graph()
{
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

void mst_prim(const char start)
{
    const comp_t comp = [](const edge_t& e1, const edge_t& e2){ return e1.second > e2.second; };
    queue_t q(comp);
    q.push({start, 0});
    while (!q.empty()) {
        const auto e = q.top();
        q.pop();
        if (g_visited[e.first - 'A'] == 0) {
            g_visited[e.first - 'A'] = 1;
            g_tree.push_back(e);
            g_weight += e.second;
            for (const auto& adj : g_graph[e.first - 'A'])
                if (g_visited[adj.first - 'A'] == 0)
                    q.push(adj);
        }
    }
}

int main(int argc, char* argv[])
{
    init_graph();
    mst_prim('A');
    // present result
    if (std::any_of(g_visited.begin(), g_visited.end(), [](const auto v){ return v == 0;}))
        printf("\nThe graph is not coherent\n");
    else {
        printf("\nEdges of minimal spanning tree of a weight %i:\n", g_weight);
        for (const auto& e : g_tree)
            printf("\t%c-%i\n", e.first, e.second);
    }
}

/*    clang++.exe -Wall -g -std=c++17 mst_prim.cpp -o mst_prim.exe

Output:

Edges of minimal spanning tree of a weight 16:
        A-0
        C-1
        G-1
        K-1
        F-2
        B-1
        M-2
        J-1
        E-2
        D-2
        H-1
        I-1
        L-1

*/

#include <iostream>
#include <limits>
#include <queue>

// for a given graph with positive length edges find
// the shortest path between two arbitrary vertexes


// case definition
using edge_t = std::pair<char, int>; 
using graph_t = std::vector<std::vector<edge_t>>;

// globals
graph_t g_graph;
std::vector<char> g_paths;
std::vector<int> g_lengths;

void add_edge(const char n1, const char n2, int len)
{
    g_graph[n1 - 'A'].emplace_back(n2, len);
    g_graph[n2 - 'A'].emplace_back(n1, len);
}

void init_graph()
{
    constexpr int vertex_count = 13;
    g_graph.resize(vertex_count);
    g_paths.resize(vertex_count);
    g_lengths.resize(vertex_count);
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
    for (int i = 0; i < vertex_count; ++i) {
        g_lengths[i] = std::numeric_limits<int>::max();
        g_paths[i] = 'A' + i;
    }
}

void find_shortest_path(char start, char stop)
{
    g_lengths[start - 'A'] = 0;
    std::deque<char> cache{start};
    while (!cache.empty()) {
        const char n = cache.front();
        cache.pop_front();
        if (n == stop) return;
        for (const edge_t& e : g_graph[n - 'A']) {
            const int start_to_e_len = g_lengths[n - 'A'] + e.second;
            if (g_lengths[e.first - 'A'] > start_to_e_len) {
                g_lengths[e.first - 'A'] = start_to_e_len;
                g_paths[e.first - 'A'] = n;
                cache.push_back(e.first);
            }
        }
    }
}

int main(int argc, char* argv[])
{
    init_graph();
    find_shortest_path('A', 'M');
    // present result
    if (g_paths['M'] == 'M')
        printf("\nPath not found\n");
    else {
        printf("\nFound shortest path from A to M with the length of %i\n", g_lengths['M' - 'A']);
        std::vector<char> tmp{'M'};
        char n = 'M';
        while (g_paths[n - 'A'] != n) {
            n = g_paths[n - 'A'];
            tmp.push_back(n);
        }
        printf("\t%c", tmp.back());
        for (auto it = std::next(tmp.rbegin()); it != tmp.rend(); ++it)
            printf("->%c", *it);
    }
}

/*    clang++.exe -Wall -g -std=c++17 shortest_path.cpp -o shortest_path.exe

Output:

Found shortest path from A to M with the length of 5
	A->C->G->K->M

*/
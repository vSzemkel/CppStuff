
#include <assert.h>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

// Maximum flow with minimal cost
// https://cp-algorithms.com/graph/min_cost_flow.html

// for simplified version without cost, see: /usaco/chapter4/ditch.cpp


/* For graphs defined with capacity and cost for each edge
struct edge_t {
    int src;
    int dst;
    int capacity;
    int cost;
}; */

constexpr auto big32 = std::numeric_limits<int>::max();

int g_V, g_E;
std::vector<std::vector<int>> g_adj, g_cost, g_capacity; // [src][dst] == attribute

static void shortest_paths(int source, std::vector<int>& distances, std::vector<int>& predecors)
{
    assert(0 <= source && source < (int)g_cost.size());
    std::vector<bool> in_queue(g_V, false);
    std::queue<int> q;
    q.push(source);
    predecors.assign(g_V, -1);
    distances.assign(g_V, big32);
    distances[source] = 0;

    while (!q.empty()) {
        const int cur = q.front(); q.pop();
        in_queue[cur] = false;
        for (const auto next : g_adj[cur])
            if (distances[cur] + g_cost[cur][next] < distances[next] && g_capacity[cur][next] > 0) {
                distances[next] = distances[cur] + g_cost[cur][next];
                predecors[next] = cur;
                if (!in_queue[next]) {
                    q.push(next);
                    in_queue[next] = true;
                }
            }
    }
}

static void max_flow_min_cost(const int source, const int target)
{
    int flow{0}, cost{0};
    std::vector<int> distances, predecors;
    while (true) { // while flow < desired
        shortest_paths(source, distances, predecors);
        if (distances[target] == big32) break;

        int path_flow{big32};
        for (int cur = target; predecors[cur] != -1; cur = predecors[cur])
            path_flow = std::min(path_flow, g_capacity[predecors[cur]][cur]);

        flow += path_flow;
        cost += distances[target];
        for (int cur = target; predecors[cur] != -1; cur = predecors[cur]) {
            g_capacity[predecors[cur]][cur] -= path_flow;
            g_capacity[cur][predecors[cur]] += path_flow;
        }
    }

    std::cout << "Maximal flow is " << flow << " with minimal cost " << cost << '\n';
}

static void read_graph(/* std::vector<edge_t> */)
{
    for (int i = 0; i < g_E; ++i) {
        char s, d; int ca, co; std::cin >> s >> d >> ca >> co;
        const int is = (int)s - 'A', id = (int)d - 'A';
        g_adj[is].push_back(id);
        g_adj[id].push_back(is);
        g_cost[is][id] = co;
        g_cost[id][is] = -co;
        g_capacity[is][id] = ca;
    }
}

void solve() {
    std::cin >> g_V >> g_E;
    g_adj.resize(g_V);
    g_cost.assign(g_V, std::vector<int>(g_V));
    g_capacity.assign(g_V, std::vector<int>(g_V));
    char src, dst; std::cin >> src >> dst;

    read_graph();
    max_flow_min_cost(src - 'A', dst - 'A');
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve();
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 max_flow_min_cost.cpp -o max_flow_min_cost.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 max_flow_min_cost.cpp -o max_flow_min_cost.o

Run:
max_flow_min_cost.exe < max_flow_min_cost.in

Input:

1
9 12
A I
A B 2 1
B C 1 1
A D 1 1
B E 2 1
C F 1 1
D E 1 1
E F 1 1
D G 1 100
E H 2 1
F I 1 1
G H 1 1
H I 2 1

Output:

Case #1: Maximal flow is 3 with minimal cost 12

*/

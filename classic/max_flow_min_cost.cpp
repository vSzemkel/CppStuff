
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

template <typename T = int>
struct flow_graph_t
{
    flow_graph_t(const int sz, const int src, const int snk) : _size(sz), _source(src), _sink(snk) {
        assert(0 <= src && src < sz && 0 <= snk && snk < sz && src != snk);
        _adj.resize(_size);
        _capacity.assign(_size, std::vector<int>(_size));
        _cost = _capacity;
    }

    void add(const int from, const int to, const T forward_cap, const int cost) {
        assert(0 <= from && from < _size && 0 <= to && to < _size);
        _adj[from].push_back(to);
        _capacity[from][to] = forward_cap;
        _cost[from][to] = cost;
        _adj[to].push_back(from);
        //_capacity[to][from] = 0; set in init
        _cost[to][from] = -cost;
    }

    bool shortest_paths()
    {
        std::vector<bool> in_queue(_size, false);
        std::queue<int> q;
        q.push(_source);
        _pred.assign(_size, -1);
        _dist.assign(_size, INF);
        _dist[_source] = 0;

        bool ret{false};
        while (!q.empty()) {
            const int cur = q.front(); q.pop();
            in_queue[cur] = false;
            for (const auto next : _adj[cur]) {
                const auto can = _dist[cur] + _cost[cur][next];
                if (_capacity[cur][next] > 0 && can < _dist[next]) {
                    ret = true;
                    _dist[next] = can;
                    _pred[next] = cur;
                    if (!in_queue[next]) {
                        q.push(next);
                        in_queue[next] = true;
                    }
                }
            }
        }

        return ret;
    }

    /**
     * @brief Computes max flow and its cost.
     * @return std::pair<flow, cost> 
     */
    std::pair<T, T> compute_max_flow_min_cost() {
        T flow{0}, cost{0};
        while (shortest_paths()) { // while _sing reachable from _source
            int path_flow{INF};
            for (int cur = _sink; ~_pred[cur]; cur = _pred[cur])
                path_flow = std::min(path_flow, _capacity[_pred[cur]][cur]);

            flow += path_flow;
            cost += _dist[_sink];
            for (int cur = _sink, prev = _pred[cur]; ~prev; cur = prev, prev = _pred[prev]) {
                _capacity[prev][cur] -= path_flow;
                _capacity[cur][prev] += path_flow;
            }
        }

        return {flow, cost};
    }

    static const T EPS = (T)1e-9;
    static const T INF = (T)1e09;

    std::vector<std::vector<int>> _adj, _capacity, _cost;
    std::vector<int> _dist, _pred;
    int _size;
    int _source;
    int _sink;
};

void solve() {
    int V, E;
    std::cin >> V >> E;
    char src, dst; std::cin >> src >> dst;
    flow_graph_t g{V, src - 'A', dst - 'A'};
    for (int z = E; z; --z) {
        char s, d; int ca, co; std::cin >> s >> d >> ca >> co;
        const int is = (int)s - 'A', id = (int)d - 'A';
        g.add(is, id, ca, co);
    }

    auto mfmc = g.compute_max_flow_min_cost();
    std::cout << "Maximal flow is " << mfmc.first << " with minimal cost " << mfmc.second << '\n';
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

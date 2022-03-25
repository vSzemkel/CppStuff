
#include <assert.h>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

// Maximum flow with minimal cost
// https://cp-algorithms.com/graph/min_cost_flow.html

// for simplified version without cost, see: /usaco/chapter4/ditch.cpp


template <typename flow_t = int, typename cost_t = int64_t>
struct flow_graph_t
{
    struct edge_t {
        int dst;
        flow_t capacity;
        cost_t cost;
    };

    flow_graph_t(const int sz, const int src, const int snk) : _adj(sz), _size(sz), _source(src), _sink(snk) {
        assert(0 <= src && src < sz && 0 <= snk && snk < sz && src != snk);
    }

    void add(const int from, const int to, const flow_t forward_cap, const cost_t cost) {
        assert(0 <= from && from < _size && 0 <= to && to < _size);
        assert(forward_cap >= 0 && cost >= 0);
        const int e = int(_edges.size());
        _edges.emplace_back(edge_t{to, forward_cap, cost});
        _edges.emplace_back(edge_t{from, 0, -cost});
        _adj[from].push_back(e);
        _adj[to].push_back(e + 1);
    }

    bool shortest_paths()
    {
        std::vector<bool> in_queue(_size, false);
        std::queue<int> q;
        q.push(_source);
        _pred.assign(_size, -1);
        _dist.assign(_size, INF);
        _dist[_source] = 0;

        while (!q.empty()) {
            const int cur = q.front(); q.pop();
            in_queue[cur] = false;
            for (const int e : _adj[cur])
                if (_edges[e].capacity > 0) {
                    const auto next = _edges[e].dst;
                    const auto can = _dist[cur] + _edges[e].cost;
                    if (can < _dist[next]) {
                        _pred[next] = e;
                        _dist[next] = can;
                        if (!in_queue[next]) {
                            q.push(next);
                            in_queue[next] = true;
                        }
                    }
                }
        }

        return _dist[_sink] < INF;
    }

    /**
     * @brief Computes max flow and its cost.
     * @return std::pair<flow, cost> 
     */
    std::pair<flow_t, cost_t> compute_max_flow_min_cost() {
        flow_t flow{0};
        cost_t cost{0};
        while (shortest_paths()) { // while _sing reachable from _source
            flow_t path_flow = std::numeric_limits<flow_t>::max();
            for (int cur = _sink; ~_pred[cur]; cur = _edges[_pred[cur] ^ 1].dst)
                path_flow = std::min(path_flow, _edges[_pred[cur]].capacity);

            flow += path_flow;
            cost += _dist[_sink] * path_flow;
            for (int cur = _sink; ~_pred[cur]; cur = _edges[_pred[cur] ^ 1].dst) {
                _edges[_pred[cur]].capacity -= path_flow;
                _edges[_pred[cur] ^ 1].capacity += path_flow;
            }
        }

        return {flow, cost};
    }

    static const flow_t EPS = 1e-9;
    static const cost_t INF = std::numeric_limits<cost_t>::max();

    std::vector<std::vector<int>> _adj; // holds index to _edges
    std::vector<edge_t> _edges;
    std::vector<cost_t> _dist;
    std::vector<int> _pred; // holds index to _edges
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

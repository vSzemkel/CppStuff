/*
ID: marcin.8
LANG: C++
TASK: milk6
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=wNTYeVgjO3m&S=milk6
*/

#include <algorithm>
#include <assert.h>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>

std::ifstream task_in("milk6.in");
std::ofstream task_out("milk6.out");

/*
 * @brief Allows counteracting edges
 */
template <typename flow_t = int, typename cost_t = int64_t>
struct flow_graph2_t
{
    struct edge_t {
        int dst;
        flow_t capacity;
        cost_t cost;
    };

    flow_graph2_t(const int sz, const int src, const int snk) : _adj(sz), _size(sz), _source(src), _sink(snk) {
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

    int add_coalesce(const int from, const int to, const flow_t forward_cap, const cost_t cost) {
        assert(0 <= from && from < _size && 0 <= to && to < _size);
        assert(forward_cap >= 0 && cost >= 0);
        for (int e : _adj[from])
            if ((e & 1) == 0 && _edges[e].dst == to) { // not residual edge
                _edges[e].capacity += forward_cap;
                _edges[e].cost += cost;
                _edges[e ^ 1].cost -= cost;
                return e;
            }

        add(from, to, forward_cap, cost);
        return -1;
    }

    void sort() {
        for (auto& ve : _adj)
            std::sort(ve.begin(), ve.end());
    }

    int edmonds_karp(bool shortcut = true) {
        std::queue<std::pair<int, flow_t>> q; // {node, flow}
        _pred.assign(_size, -1);
        _pred[_source] = -2;
        q.push({_source, std::numeric_limits<flow_t>::max()});

        while (!q.empty()) {
            const auto cur = q.front().first;
            const auto flow = q.front().second;
            q.pop();

            for (const int ne : _adj[cur])
                if (_edges[ne].capacity) {
                    const int next = _edges[ne].dst;
                    if (_pred[next] == -1) {
                        _pred[next] = ne;
                        const auto nflow = std::min(flow, _edges[ne].capacity);
                        if (shortcut && next == _sink)
                            return nflow;
                        q.push({next, nflow});
                    }
                }
        }

        return 0;
    }

    /**
     * @brief Computes max flow
     */
    flow_t compute_max_flow() {
        flow_t flow{0};
        while (flow_t path_flow = edmonds_karp()) { // while _sink reachable from _source
            flow += path_flow;
            for (int cur = _sink; _pred[cur] >= 0; cur = _edges[_pred[cur] ^ 1].dst) {
                _edges[_pred[cur]].capacity -= path_flow;
                _edges[_pred[cur] ^ 1].capacity += path_flow;
            }
        }

        return flow;
    }

    inline static const flow_t EPS = 1e-9;
    inline static const cost_t INF = std::numeric_limits<cost_t>::max();

    std::vector<std::vector<int>> _adj; // holds index to _edges
    std::vector<edge_t> _edges;
    std::vector<cost_t> _dist;
    std::vector<int> _pred; // holds index to _edges
    int _size;
    int _source;
    int _sink;
};

struct edge_t {
    int cost;
    int coalesced;
    int embark;
    int index;
    int src;
    int dst;
    bool operator<(const edge_t& other) {
        return cost < other.cost
           || (cost == other.cost && coalesced < other.coalesced)
           || (cost == other.cost && coalesced == other.coalesced && embark < other.embark);
    }
};

int main(int, char**)
{
    int V, E;
    task_in >> V >> E;
    flow_graph2_t<int, int64_t> fg{V, 0, V - 1};
    std::unordered_map<int, std::vector<int>> coalesced; // {primary route -> coalesced routes}
    std::vector<edge_t> edges;
    int dub{0};
    for (int i = 0; i < E; ++i) {
        int f, t, c; task_in >> f >> t >> c;
        --f; --t;
        const auto prim = fg.add_coalesce(f, t, 1, c);
        if (~prim) {
            ++dub;
            ++edges[prim >> 1].coalesced;
            coalesced[prim >> 1].push_back(i);
        } else
            edges.emplace_back(edge_t{c, 0, 0, i - dub, f, t});
    }

    fg.sort();
    fg.edmonds_karp(false);
    for (edge_t& e : edges) {
        int n = e.dst;
        while (~fg._pred[n] && fg._edges[fg._pred[n] ^ 1].dst != fg._source) 
            n = fg._edges[fg._pred[n] ^ 1].dst;
        if (~fg._pred[n])
            e.embark = fg._pred[n] >> 1;
    }
    std::sort(edges.begin(), edges.end());

    int cost{0};
    auto copy_fg = fg;
    std::vector<int> routes;
    int flow = fg.compute_max_flow();
    while (flow) {
        for (const edge_t& e : edges) {
            const auto ind = e.index << 1;
            int& oc = copy_fg._edges[ind].capacity;
            if (oc == 0)
                continue;
            fg = copy_fg;
            fg._edges[ind].capacity = 0;
            const int can = fg.compute_max_flow();
            if (can < flow) {
                oc = 0;
                routes.push_back(e.index);
                cost += fg._edges[ind].cost;
                flow = can;
                break;
            }
        }
    }

    const int rsz = int(routes.size());
    for (int i = 0; i < rsz; ++i) {
        const auto& c = coalesced[routes[i]];
        routes.insert(routes.end(), c.begin(), c.end());
    }
    task_out << cost << ' ' << routes.size() << '\n';
    std::sort(routes.begin(), routes.end());
    for (const int r : routes)
        task_out << r + 1 << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 milk6.cpp -o milk6.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address milk6.cpp -o milk6

Input:

4 5
1 3 100
3 2 50
2 4 60
1 2 40
2 3 80

Output:

60 1
3

*/

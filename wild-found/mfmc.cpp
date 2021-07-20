#include <bits/extc++.h>

// requires: __gnu_pbds::priority_queue

template <typename flow_t = int, typename cost_t = long long>
struct MFMC // by ecnerwala
{
    struct edge_t
    {
        int dest;
        flow_t cap;
        cost_t cost;
    };

    std::vector<std::vector<int>> adj;
    std::vector<edge_t> edges;
    std::vector<char> seen;
    std::vector<cost_t> pi;
    std::vector<int> prv;
    int N;

    explicit MFMC(int N_) : N(N_), adj(N), pi(N, 0), prv(N) {}

    void addEdge(int from, int to, flow_t cap, cost_t cost)
    {
        assert(cap >= 0);
        assert(cost >= 0); // TODO: Remove this restriction
        int e = int(edges.size());
        edges.emplace_back(edge_t{to, cap, cost});
        edges.emplace_back(edge_t{from, 0, -cost});
        adj[from].push_back(e);
        adj[to].push_back(e + 1);
    }

    const cost_t INF_COST = std::numeric_limits<cost_t>::max() / 4;
    const flow_t INF_FLOW = std::numeric_limits<flow_t>::max() / 4;
    std::vector<cost_t> dist;
    __gnu_pbds::priority_queue<std::pair<cost_t, int>> q;
    std::vector<typename decltype(q)::point_iterator> its;
    void path(int s)
    {
        dist.assign(N, INF_COST);
        dist[s] = 0;

        its.assign(N, q.end());
        its[s] = q.push({0, s});

        while (!q.empty()) {
            int i = q.top().second;
            q.pop();
            cost_t d = dist[i];
            for (int e : adj[i]) {
                if (edges[e].cap) {
                    int j = edges[e].dest;
                    cost_t nd = d + edges[e].cost;
                    if (nd < dist[j]) {
                        dist[j] = nd;
                        prv[j] = e;
                        if (its[j] == q.end()) {
                            its[j] = q.push({-(dist[j] - pi[j]), j});
                        } else {
                            q.modify(its[j], {-(dist[j] - pi[j]), j});
                        }
                    }
                }
            }
        }

        swap(pi, dist);
    }

    std::vector<std::pair<flow_t, cost_t>> maxflow(int s, int t)
    {
        assert(s != t);
        std::vector<std::pair<flow_t, cost_t>> res;
        flow_t totFlow = 0;
        cost_t totCost = 0;
        while (path(s), pi[t] < INF_COST) {
            flow_t curFlow = std::numeric_limits<flow_t>::max();
            for (int cur = t; cur != s;) {
                int e = prv[cur];
                int nxt = edges[e ^ 1].dest;
                curFlow = std::min(curFlow, edges[e].cap);
                cur = nxt;
            }
            res.push_back({curFlow, pi[t]});
            totFlow += curFlow;
            totCost += pi[t] * curFlow;
            for (int cur = t; cur != s;) {
                int e = prv[cur];
                int nxt = edges[e ^ 1].dest;
                edges[e].cap -= curFlow;
                edges[e ^ 1].cap += curFlow;
                cur = nxt;
            }
        }
        return res;
    }
};

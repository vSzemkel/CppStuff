/*
ID: marcin.8
LANG: C++
TASK: job
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=BxiyJvhIDID&S=job
*/

#include <algorithm>
#include <cassert>
#include <fstream>
#include <numeric>
#include <queue>
#include <vector>

std::ifstream task_in("job.in");
std::ofstream task_out("job.out");

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
            for (const auto next : _adj[cur])
                if (_dist[cur] + _cost[cur][next] < _dist[next] && _capacity[cur][next] > 0) {
                    ret = true;
                    _dist[next] = _dist[cur] + _cost[cur][next];
                    _pred[next] = cur;
                    if (!in_queue[next]) {
                        q.push(next);
                        in_queue[next] = true;
                    }
                }
        }

        return ret;
    }

    std::pair<T, T> compute_max_flow_min_cost() {
        T flow{0}, cost{0};
        while (shortest_paths()) { // while _sing reachable from _source
            int path_flow{INF};
            for (int cur = _sink; ~_pred[cur]; cur = _pred[cur])
                path_flow = std::min(path_flow, _capacity[_pred[cur]][cur]);

            flow += path_flow;
            cost = std::max(cost, _dist[_sink]);
            for (int cur = _sink, prev = _pred[cur]; ~prev; cur = prev, prev = _pred[prev]) {
                _capacity[prev][cur] -= path_flow;
                _capacity[cur][prev] += path_flow;
            }
        }

        return {flow, cost};
    }

    static constexpr T EPS = (T)1e-9;
    static constexpr T INF = (T)1e09;

    std::vector<std::vector<int>> _adj, _capacity, _cost;
    std::vector<int> _dist, _pred;
    int _size;
    int _source;
    int _sink;
};

int main(int, char**)
{
    int N, A, B;
    task_in >> N >> A >> B;

    // {completion, machine latency}
    std::vector<int> machinesA(A), completionA(N), machinesB(B);
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
    auto p = std::make_pair(0, 0);
    for (int i = 0; i < A; ++i) {
        auto& a = machinesA[i];
        task_in >> a;
        pq.push({a, i});
    }
    for (auto& b : machinesB)
        task_in >> b;

    for (int z = N - 1; z; --z) {
        p = pq.top(); pq.pop();
        completionA[z] = p.first;
        p.first += machinesA[p.second];
        pq.push(p);
    }
    p = pq.top();
    completionA[0] = p.first;
    task_out << p.first << ' ';

    // model a graph with all internal edges with unit capacity
    const int S = N + B; // start
    const int F = S + 1; // finish
    const int V = S + 2; // size
    flow_graph_t<int> g{V, S, F};
    for (int i = 0; i < N; ++i) {
        g.add(S, i, 1, completionA[i]);
        for (int j = 0; j < B; ++j)
            g.add(i, N + j, 1, 0);
    }
    for (int j = 0; j < B; ++j)
        g.add(N + j, F, N, machinesB[j]);

    task_out << g.compute_max_flow_min_cost().second << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 job.cpp -o job.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address job.cpp -o job

Input:


Output:


*/

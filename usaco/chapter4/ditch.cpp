/*
ID: marcin.8
LANG: C++
TASK: ditch
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=fgRQq5qK6UD&S=ditch
*/

#include <algorithm>
#include <cassert>
#include <fstream>
#include <queue>
#include <vector>

std::ifstream task_in("ditch.in");
std::ofstream task_out("ditch.out");

template <typename T = int>
struct flow_graph_t
{
    flow_graph_t(const int sz, const int src, const int snk) : _size(sz), _source(src), _sink(snk) {
        assert(0 <= src && src < sz && 0 <= snk && snk < sz && src != snk);
        _adj.resize(_size);
        _pred.resize(_size);
        _capacity.assign(_size, std::vector<int>(_size));
    }

    void add(const int from, const int to, const T forward_cap) {
        assert(0 <= from && from < _size && 0 <= to && to < _size);
        _adj[from].push_back(to);
        _capacity[from][to] += forward_cap;
        _adj[to].push_back(from);
        // _capacity[to][from] = 0; set in init
    }

    bool bfs() {
        std::queue<int> q;
        q.push(_source);
        _pred.assign(_size, -1);

        while (!q.empty()) {
            const int cur = q.front(); q.pop();
            if (cur == _sink)
                return true;
            for (const auto next : _adj[cur])
                if (next != _source && _pred[next] == -1 && _capacity[cur][next] > 0) {
                    _pred[next] = cur;
                    q.push(next);
                }
        }

        return false;
    }

    T compute_max_flow() {
        T flow{0};
        while (bfs()) { // while _sing reachable from _source
            int path_flow{INF};
            for (int cur = _sink; ~_pred[cur]; cur = _pred[cur])
                path_flow = std::min(path_flow, _capacity[_pred[cur]][cur]);

            flow += path_flow;
            for (int cur = _sink, prev = _pred[cur]; ~prev; cur = prev, prev = _pred[prev]) {
                _capacity[prev][cur] -= path_flow;
                _capacity[cur][prev] += path_flow;
            }
        }

        return flow;
    }

    static constexpr T EPS = (T)1e-9;
    static constexpr T INF = (T)1e09;

    std::vector<std::vector<int>> _adj, _capacity;
    std::vector<int> _pred;
    int _size;
    int _source;
    int _sink;
};

int main(int, char**)
{
    int E, V; task_in >> E >> V;
    flow_graph_t<int> fg(V, 0, V - 1);
    for (int i = 0; i < E; ++i) {
        int f, t, c;
        task_in >> f >> t >> c;
        fg.add(--f, --t, c);
    }

    task_out << fg.compute_max_flow() << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 ditch.cpp -o ditch.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address ditch.cpp -o ditch

Run:
ditch.exe < ditch.in

Input:

5 4
1 2 40
1 4 20
2 4 20
2 3 30
3 4 10

Output:

50

*/

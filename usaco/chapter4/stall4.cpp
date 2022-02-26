/*
ID: marcin.8
LANG: C++
TASK: stall4
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=akC2pFr0a6m&S=stall4
*/

#include <fstream>
#include <limits>
#include <queue>
#include <vector>

std::ifstream task_in("stall4.in");
std::ofstream task_out("stall4.out");

class hopcroft_karp_t { // based on https://www.geeksforgeeks.org/hopcroft-karp-algorithm-for-maximum-matching-set-2-implementation/?ref=rp
  public:
    hopcroft_karp_t(const int C, const int M) : _C(C), _M(M), _adj(C + 1) {}

    void add_edge(const int c, const int m) { _adj[c + 1].push_back(m + 1); }

    int compute() {
        _dist.resize(_C + 1);
        _pair_c.resize(_C + 1);
        _pair_m.resize(_M + 1);

        int ret{0};
        while (bfs())
            for (int c = 1; c <= _C; ++c)
                if (_pair_c[c] == SENTINEL && dfs(c))
                    ++ret;
        return ret;
    }

  private:
    int _C, _M; // candidates -> matches
    std::vector<std::vector<int>> _adj;
    std::vector<int> _pair_c, _pair_m, _dist;
    static constexpr int SENTINEL = 0;
    static constexpr int INF = std::numeric_limits<int>::max();

    bool bfs() {
        std::queue<int> q;
        for (int c = 1; c <= _C; ++c)
            if (_pair_c[c] == SENTINEL) {
                _dist[c] = 0;
                q.push(c);
            } else
                _dist[c] = INF;

        _dist[SENTINEL] = INF; // distance to sentinel

        while (!q.empty()) {
            const int c = q.front(); q.pop();
            if (_dist[c] < _dist[SENTINEL])
                for (const int m : _adj[c]) {
                    const auto cdt = _pair_m[m];
                    if (_dist[cdt] == INF) {
                        _dist[cdt] = _dist[c] + 1;
                        q.push(cdt);
                    }
                }
        }

        return _dist[SENTINEL] < INF;
    }

    bool dfs(const int c) {
        if (c == 0)
            return true;

        for (const int m : _adj[c])
            if (_dist[_pair_m[m]] == _dist[c] + 1 && dfs(_pair_m[m])) {
                _pair_c[c] = m;
                _pair_m[m] = c;
                return true;
            }

        _dist[c] = INF;
        return false;
    }
};

int main(int, char**)
{
    int N, M;
    task_in >> N >> M;

    hopcroft_karp_t hc(N, M);
    for (int c = 0; c < N; ++c) {
        int z; task_in >> z;
        for (int i = z; i; --i) {
            int s; task_in >> s;
            hc.add_edge(c, --s);
        }
    }

    task_out << hc.compute() << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 stall4.cpp -o stall4.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address stall4.cpp -o stall4

Input:


Output:


*/

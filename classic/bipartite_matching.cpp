
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

// Bipartite graph maximal matching using Hopcroft-Karp algorithm
// for simpler solution see max_matching in /codejam/2021/retiling.cpp

using bipartite_t = std::vector<std::vector<bool>>;

class hopcroft_karp_t { // based on https://www.geeksforgeeks.org/hopcroft-karp-algorithm-for-maximum-matching-set-2-implementation/?ref=rp
  public:
    hopcroft_karp_t(const int C, const int M) : _C(C), _M(M), _adj(C + 1) {}
    hopcroft_karp_t(const bipartite_t& bg) {
        if (bg.empty())
            _C = _M = 0;
        else {
            _C = bg.size();
            _M = bg[0].size();
            _adj.resize(_C + 1);
        }

        for (int r = 0; r < _C; ++r)
            for (int c = 0; c < _M; ++c)
                if (bg[r][c]) add_edge(r, c);
    }
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
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    hopcroft_karp_t g(4, 4);
    g.add_edge(0, 1);
    g.add_edge(0, 2);
    g.add_edge(1, 0);
    g.add_edge(2, 1);
    g.add_edge(3, 1);
    g.add_edge(3, 3);
 
    std::cout << "Size of maximum matching is " << g.compute() << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 bipartite_matching.cpp -o bipartite_matching.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address bipartite_matching.cpp -o bipartite_matching

Run:
bipartite_matching.exe

*/

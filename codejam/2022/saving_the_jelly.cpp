
#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <filesystem>
#include <fstream>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <vector>

// Saving the Jelly
// https://codingcompetitions.withgoogle.com/codejam/round/00000000008778ec/0000000000b158f8

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

    int _C, _M; // candidates -> matches
    std::vector<std::vector<int>> _adj;
    std::vector<int> _pair_c, _pair_m, _dist;
  private:
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

static void solve() {
    int N; std::cin >> N;
    std::vector<std::pair<int, int>> children(N), jellys(N + 1);
    for (auto& c : children)
        std::cin >> c.first >> c.second;
    for (auto& j : jellys)
        std::cin >> j.first >> j.second;

    std::vector<std::vector<int64_t>> dist(N, std::vector<int64_t>(N + 1));
    for (int c = 0; c < N; ++c)
        for (int j = 0; j <= N; ++j) {
            const int64_t dx = children[c].first - jellys[j].first;
            const int64_t dy = children[c].second - jellys[j].second; 
            dist[c][j] = dx * dx + dy * dy;
        }

    hopcroft_karp_t g(N, N + 1);
    for (int c = 0; c < N; ++c) {
        const auto& d = dist[c];
        const auto mindist = *std::min_element(d.begin(), d.end());
            for (int j = 0; j <= N; ++j)
                if (dist[c][j] == mindist)
                    g.add_edge(c, j);
    }

    g.compute();

    if (g._pair_m[1])
        std::cout << "IMPOSSIBLE";
    else {
        std::vector<int> order(N);
        std::iota(order.begin(), order.end(), 0);
        std::sort(order.begin(), order.end(), [&](const int c1, const int c2){
            return dist[c1][g._pair_c[c1 + 1]] <  dist[c2][g._pair_c[c2 + 1]];
        });
        std::cout << "POSSIBLE\n";
        for (const int c : order)
            std::cout << c << ' ' << g._pair_c[c] << '\n';
    }
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 saving_the_jelly.cpp -o saving_the_jelly.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address saving_the_jelly.cpp -o saving_the_jelly

Run:
saving_the_jelly.exe < saving_the_jelly.in

Input:

4
2
-3 0
-1 0
3 0
-2 -1
-2 1
1
0 0
1 1
2 2
3
10 0
-10 0
0 0
0 5
-1 0
5 0
0 -5
2
3 4
3 4
5 7
3 4
5 7

Output:


*/

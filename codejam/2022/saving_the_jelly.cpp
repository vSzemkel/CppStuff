
#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

// Saving the Jelly
// https://codingcompetitions.withgoogle.com/codejam/round/00000000008778ec/0000000000b158f8

class hopcroft_karp_t { // based on https://www.geeksforgeeks.org/hopcroft-karp-algorithm-for-maximum-matching-set-2-implementation/?ref=rp
  public:
    hopcroft_karp_t(const int C, const int M) : _C(C), _M(M), _adj(C + 1) {}

    void add_edge(const int c, const int m) { _adj[c + 1].push_back(m + 1); }

    int m_for_c(const int i) const { // match for candidate (c->m)
        return _pair_c[i + 1] - 1;
    }

    int c_for_m(const int i) const { // candidate matched with match (m->c)
        return _pair_m[i + 1] - 1;
    }

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

static void solve() {
    int N; std::cin >> N;
    std::vector<std::pair<int, int>> children(N), jellys(N);
    for (auto& c : children)
        std::cin >> c.first >> c.second;
    int bx, by; std::cin >> bx >> by;
    for (auto& j : jellys)
        std::cin >> j.first >> j.second;
    jellys.emplace_back(bx, by);

    // for each child compute order of closest jellys
    std::vector<std::vector<int>> order(N, std::vector<int>(N + 1)), positions = order;
    for (int c = 0; c < N; ++c) {
        auto& ord = order[c];
        auto& pos = positions[c];
        for (int j = 0; j <= N; ++j)
            ord[j] = j;
        std::stable_sort(ord.begin(), ord.end(), [&](const int j1, const int j2) {
            const int64_t dx1 = children[c].first - jellys[j1].first;
            const int64_t dy1 = children[c].second - jellys[j1].second;
            const int64_t dx2 = children[c].first - jellys[j2].first;
            const int64_t dy2 = children[c].second - jellys[j2].second;
            return dx1 * dx1 + dy1 * dy1 < dx2 * dx2 + dy2 * dy2;
        });
        for (int j = 0; j <= N; ++j)
            pos[ord[j]] = j;
    }


    // for each child consider jellys not farther then blueberry one
    hopcroft_karp_t g(N, N);
    for (int c = 0; c < N; ++c) {
        const auto& ord = order[c];
        const int bb_pos = positions[c][N];
        for (int j = 0; j < bb_pos; ++j)
            g.add_edge(c, ord[j]);
    }

    if (g.compute() < N)
        std::cout << "IMPOSSIBLE";
    else {
        std::vector<std::pair<int, int>> ans;
        std::vector<bool> used_c(N), used_j = used_c;
        std::vector<int> cidx(N); // next jelly to consider for each child
        while (int(ans.size()) < N) {
            for (int c = 0; c < N; ++c)
                while (used_j[order[c][cidx[c]]]) ++cidx[c];

            int c{0}, j{-1};
            while (used_c[c])
                ++c;
            std::vector<bool> seen(N);
            while (true) {
                seen[c] = true;
                j = order[c][cidx[c]];
                assert(!used_j[j]);
                c = g.c_for_m(j);
                assert(!used_c[c]);
                if (seen[c])
                    break;
            }

            const int found_c = c;
            while (true) { // there is no guarantee that child is matched with the closest jelly, but we can alter the matching to be so
                j = order[c][cidx[c]];
                assert(!used_j[j]);
                used_j[j] = true;
                const int nc =  g.c_for_m(j);
                assert(!used_c[nc]);
                used_c[nc] = true;
                ans.emplace_back(c, j);
                c = nc;
                if (c == found_c)
                    break;
            }
        }

        std::cout << "POSSIBLE";
        for (const auto& [c, j] : ans)
            std::cout << '\n' << c + 1 << ' ' << j + 2;
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

Case #1: POSSIBLE
1 2
2 3
Case #2: IMPOSSIBLE
Case #3: POSSIBLE
1 3
2 2
3 4
Case #4: POSSIBLE
1 2
2 3

*/

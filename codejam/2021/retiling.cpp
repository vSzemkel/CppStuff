
#include <algorithm>
#include <bits/extc++.h>
#include <cassert>
#include <iostream>
#include <limits>
#include <queue>
#include <set>
#include <string>
#include <vector>

// Retiling
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000435915/00000000007dc2de

template <typename flow_t = int, typename cost_t = long long>
struct MCMF_SSPA
{
    int N;
    std::vector<std::vector<int>> adj;
    struct edge_t
    {
        int dest;
        flow_t cap;
        cost_t cost;
    };
    std::vector<edge_t> edges;

    std::vector<char> seen;
    std::vector<cost_t> pi;
    std::vector<int> prv;

    explicit MCMF_SSPA(int N_) : N(N_), adj(N), pi(N, 0), prv(N) {}

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

static void solve() { // by ecnerwala
    int R, C, FC, SC; std::cin >> R >> C >> FC >> SC;
    std::vector<std::string> start(R), final(R);
    for (auto& r : start)
        std::cin >> r;
    for (auto& r : final)
        std::cin >> r;

    const int S = R * C;
    const int T = S + 1;
    const int V = S + 2;
    const int INF = 2 * S;
    MCMF_SSPA flower(V);

    int num_bad{0};
    for (int r = 0; r < R; ++r) 
        for (int c = 0; c < C; ++c) {
            const auto ind = r * C + c;
            if (start[r][c] != final[r][c]) {
                ++num_bad;
                if (start[r][c] == 'M')
                    flower.addEdge(S, ind, 1, 0);
                else
                    flower.addEdge(ind, T, 1, 0);
            }

            if (r) {
                flower.addEdge(ind - C, ind, INF, 1);
                flower.addEdge(ind, ind - C, INF, 1);
            }
            if (c) {
                flower.addEdge(ind - 1, ind, INF, 1);
                flower.addEdge(ind, ind - 1, INF, 1);
            }
        }

    auto flows = flower.maxflow(S, T);
    int64_t ans = num_bad * FC;
    for (auto [f, c] : flows) {
        c = c * SC - 2 * FC;
        if (c >= 0) break;
        ans += f * int64_t(c);
    }

    std::cout << ans;
}

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

static bool bpm(const bipartite_t& bg, const int candidate, std::vector<int>& matched, std::vector<bool>& seen) {
    const int match_size = int(bg[0].size());
    for (int m = 0; m < match_size; ++m)
        if (bg[candidate][m] && !seen[m]) {
            seen[m] = true;
            if (matched[m] < 0 || bpm(bg, matched[m], matched, seen)) {
                matched[m] = candidate;
                return true;
            }
        }
    return false;
}

static int max_matching(const bipartite_t& bg) {
    if (bg.empty() || bg[0].empty())
        return 0;
    std::vector<int> matched(bg[0].size(), -1);
    int ret{0};
    for (int i = 0; i < int(bg.size()); ++i) {
        std::vector<bool> seen(matched.size());
        if (bpm(bg, i, matched, seen))
            ++ret;
    }
    return ret;
}

static void solve_set1() { // need maximum matching in bipartite graph for this
    int R, C, F, S; std::cin >> R >> C >> F >> S;
    std::vector<std::string> start(R), final(R);
    for (auto& r : start)
        std::cin >> r;
    for (auto& r : final)
        std::cin >> r;

    // construct bipartite graph
    int M{0}, G{0}, flips{0};
    std::vector<std::vector<int>> order(R, std::vector<int>(C));
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c)
            if (start[r][c] == 'M') 
                order[r][c] = M++;
            else
                order[r][c] = G++;
    bipartite_t bg(M, std::vector<bool>(G)); // M -> G
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c) {
            if (start[r][c] == final[r][c])
                continue;
            if (c < C - 1 && start[r][c] != start[r][c + 1] && final[r][c] != final[r][c + 1]) {
                if (start[r][c] == 'M')
                    bg[order[r][c]][order[r][c + 1]] = true;
                else
                    bg[order[r][c + 1]][order[r][c]] = true;
            }
            if (r < R - 1 && start[r][c] != start[r + 1][c] && final[r][c] != final[r + 1][c]) {
                if (start[r][c] == 'M')
                    bg[order[r][c]][order[r + 1][c]] = true;
                else
                    bg[order[r + 1][c]][order[r][c]] = true;
            }
            ++flips;
        }

    //int swaps = max_matching(bg);
    hopcroft_karp_t hk(bg);
    int swaps = hk.compute();
    swaps = std::min(swaps, flips / 2);
    std::cout << flips - swaps;
}

static void solve_set1_incorrect() { // need maximum matching in bipartite graph for this
    int R, C, F, S; std::cin >> R >> C >> F >> S;
    std::vector<std::string> start(R), final(R);
    for (auto& r : start)
        std::cin >> r;
    for (auto& r : final)
        std::cin >> r;

    int flips{0}, swapsh{0}, swapsv{0};
    std::set<int> tailsh, tailsv;
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c) {
            if (start[r][c] == final[r][c])
                continue;
            const int pos = r * C + c;
            if (tailsh.count(pos) == 0 && tailsv.count(pos) == 0) {
                if (c < C - 1 && tailsv.count(pos + 1) == 0 && start[r][c] != start[r][c + 1] && final[r][c] != final[r][c + 1]) {
                    tailsh.insert(pos + 1);
                    ++swapsh;
                } else if (r < R - 1 && tailsh.count(pos + C) == 0 && start[r][c] != start[r + 1][c] && final[r][c] != final[r + 1][c]) {
                    tailsv.insert(pos + C);
                    ++swapsv;
                }
            }
            ++flips;
        }

    std::vector<int> tails;
    std::set_intersection(tailsh.begin(), tailsh.end(), tailsv.begin(), tailsv.end(), std::back_inserter(tails));

    const int swaps = std::min(swapsh + swapsv - int(tails.size()), flips / 2);
    std::cout << flips - swaps;
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 retiling.cpp -o retiling.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address retiling.cpp -o retiling

Run:
retiling.exe < retiling.in

Input:

3
2 4 1 1
MGMG
MMMG
GMGM
MMMM
3 3 1 1
MGG
GMG
MMM
MMM
MGM
MMG
1 5 1000 1
MGGGG
GGGMM

Output:

*/

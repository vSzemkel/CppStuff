
#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <numeric>
#include <optional>
#include <utility>
#include <tuple>
#include <vector>

// Square Free
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000436142/0000000000813e1a#analysis

template <typename T>
class flow_graph_t
{
  public:
    static constexpr T eps = (T)1e-9;

    struct edge_t {
        int from;
        int to;
        T capacity;
        T flow;
    };

    std::vector<std::vector<int>> g;
    std::vector<edge_t> edges;
    int n;
    int st;
    int fin;
    T flow;

    flow_graph_t(int _n, int _st, int _fin) : n(_n), st(_st), fin(_fin) {
        assert(0 <= st && st < n && 0 <= fin && fin < n && st != fin);
        g.resize(n);
        flow = 0;
    }

    void clear_flow() {
        flow = 0;
        for (const auto& e : edges)
            e.flow = 0;
    }

    int add(int from, int to, T forward_cap, T backward_cap = T{}) {
        assert(0 <= from && from < n && 0 <= to && to < n);
        int id = (int)edges.size();
        g[from].push_back(id);
        edges.push_back({from, to, forward_cap, 0});
        g[to].push_back(id + 1);
        edges.push_back({to, from, backward_cap, 0});
        return id;
    }
};

template <typename T>
class dinic_t
{
  public:
    flow_graph_t<T>& g;

    std::vector<int> ptr;
    std::vector<int> d;
    std::vector<int> q;

    dinic_t(flow_graph_t<T>& _g) : g(_g) {
        ptr.resize(g.n);
        d.resize(g.n);
        q.resize(g.n);
    }

    bool expath() {
        fill(d.begin(), d.end(), -1);
        q[0] = g.fin;
        d[g.fin] = 0;
        int beg = 0, end = 1;
        while (beg < end) {
            int i = q[beg++];
            for (int id : g.g[i]) {
                const auto& e = g.edges[id];
                const auto& back = g.edges[id ^ 1];
                if (back.capacity - back.flow > g.eps && d[e.to] == -1) {
                    d[e.to] = d[i] + 1;
                    if (e.to == g.st) {
                        return true;
                    }
                    q[end++] = e.to;
                }
            }
        }
        return false;
    }

    T dfs(int v, T w) {
        if (v == g.fin) {
            return w;
        }
        int& j = ptr[v];
        while (j >= 0) {
            int id = g.g[v][j];
            const auto& e = g.edges[id];
            if (e.capacity - e.flow > g.eps && d[e.to] == d[v] - 1) {
                T t = dfs(e.to, std::min(e.capacity - e.flow, w));
                if (t > g.eps) {
                    g.edges[id].flow += t;
                    g.edges[id ^ 1].flow -= t;
                    return t;
                }
            }
            --j;
        }
        return 0;
    }

    T max_flow() {
        while (expath()) {
            for (int i = 0; i < g.n; ++i) {
                ptr[i] = (int)g.g[i].size() - 1;
            }
            T big_add = 0;
            while (true) {
                T add = dfs(g.st, std::numeric_limits<T>::max());
                if (add <= g.eps) {
                    break;
                }
                big_add += add;
            }
            if (big_add <= g.eps) {
                break;
            }
            g.flow += big_add;
        }
        return g.flow;
    }

    std::vector<bool> min_cut() {
        max_flow();
        std::vector<bool> ret(g.n);
        for (int i = 0; i < g.n; ++i) {
            ret[i] = (d[i] != -1);
        }
        return ret;
    }
};

int R, C, MAXLEN, SIZE;
std::vector<std::string> board;
std::vector<int> rlimits, climits;

static bool generate() // inspired by tourist
{
    const int S = R + C;
    const int T = S + 1;
    const int N = S + 2;
    flow_graph_t<int> g{N, S, T};

    std::vector<int> to_rows(R);
    for (int r = 0; r < R; ++r)
        to_rows[r] = g.add(S, r, rlimits[r]);
    std::vector<int> from_cols(C);
    for (int c = 0; c < C; ++c)
        from_cols[c] = g.add(R + c, T, climits[c]);
    std::vector<std::vector<int>> cell_edges(R, std::vector<int>(C));
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c)
            cell_edges[r][c] = g.add(r, R + c, 1);

    dinic_t<int> d{g};
    d.max_flow();

    for (int r = 0; r < R; ++r)
        if (g.edges[to_rows[r]].flow != rlimits[r])
            return false;
    for (int c = 0; c < C; ++c)
        if (g.edges[from_cols[c]].flow != climits[c])
            return false;

    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c)
            board[r][c] = g.edges[cell_edges[r][c]].flow ? '/' : '\\';
    return true;
}

static std::optional<std::tuple<int, int, int>> is_square_free()
{
    std::vector<std::vector<std::pair<int, int>>> memo; // {count of '\' above, count of '/' above}
    memo.assign(R, std::vector<std::pair<int, int>>(C));

    for (int c = 0; c < C; ++c)
        memo[0][c] = (board[0][c] == '/') ? std::pair{0, 1} : std::pair{1, 0};
    for (int r = 1; r < R; ++r) {
        // update memo
        for (int c = 0; c < C; ++c) {
            const bool forward = board[r][c] == '/';
            int left = forward ? 0 : 1;
            int right = forward ? 1 : 0;
            if (left > 0 && c > 0 && board[r - 1][c - 1] != '/')
                left += memo[r - 1][c - 1].first;
            if (right > 0 && c < C - 1 && board[r - 1][c + 1] == '/')
                right += memo[r - 1][c + 1].second;
            memo[r][c] = {left, right};
        }
        // search for square
        const int bound = std::min(MAXLEN, (r + 1) / 2);
        for (int len = 1; len <= bound; ++len)
            for (int c = len; c <= C - len; ++c)
                if (memo[r][c - 1].first >= len && memo[r][c].second >= len && memo[r - len][c - len].second >= len && memo[r - len][c + len - 1].first >= len)
                    return std::tuple{r, c, len};
    }

    return std::nullopt;
}

static void solve()
{
    std::cin >> R >> C;
    SIZE = R * C;
    MAXLEN = std::min(R, C);
    rlimits.assign(R, 0);
    for (auto& r : rlimits)
        std::cin >> r;
    climits.assign(C, 0);
    for (auto& c : climits)
        std::cin >> c;
    board.assign(R, std::string(C, ' '));

    const auto rtot = std::accumulate(rlimits.begin(), rlimits.end(), 0);
    const auto ctot = std::accumulate(climits.begin(), climits.end(), 0);
    if (rtot != ctot || !generate()) {
        std::cout << "IMPOSSIBLE\n";
        return;
    }

    std::optional<std::tuple<int, int, int>> config;
    while ((config = is_square_free()) != std::nullopt) {
        const auto& cfg = config.value();
        const int r = std::get<0>(cfg);
        const int c = std::get<1>(cfg);
        const int u = r - 2 * std::get<2>(cfg) + 1;
        std::swap(board[r][c - 1], board[r][c]);
        std::swap(board[u][c - 1], board[u][c]);
    }

    std::cout << "POSSIBLE\n";
    for (const auto& s : board)
        std::cout << s << '\n';
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 square_free.cpp -o square_free.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address square_free.cpp -o square_free

Run:
square_free.exe < square_free.in

Input:

4
4 4
3 2 3 3
3 3 2 3
2 3
1 1
1 1 1
2 3
1 2
1 1 1
3 3
2 0 2
2 0 2

Output:

Case #1: POSSIBLE
/\//
//\\
//\/
\///

Case #2: IMPOSSIBLE

Case #3: POSSIBLE
\\/
//\

Case #4: POSSIBLE
/\/
\\\
/\/

*/


#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <numeric>
#include <queue>
#include <utility>
#include <vector>

// Pizza Delivery
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb0f5/0000000000ba86e6

using toll_t = std::pair<char, int>;
constexpr int N{0}, E{1}, W{2}, S{3};
constexpr int DR[] = {-1, 0, 0, +1};
constexpr int DC[] = {0, +1, -1, 0};

/**
 * Observation1: Integer division in C++ is intricate
 * Observation2: This can easily overflow 2^31
 */

inline static int64_t floor_div(const int64_t a, const int64_t b) {
    return a / b - ((a ^ b) < 0 && a % b != 0);
}

static int64_t calculate_toll(const int64_t cur, const toll_t& toll) {
    switch (toll.first) {
        case '+': return cur + toll.second;
        case '-': return cur - toll.second;
        case '*': return cur * toll.second;
        case '/': return floor_div(cur, toll.second);
    }

    return 0;
}

static void solve() { // neal_wu
    int L, P, M, R, C;
    std::cin >> L >> P >> M >> R >> C;
    std::array<toll_t, 4> tolls;
    for (auto& [op, fac] : tolls)
        std::cin >> op >> fac;

    const int L2 = L * L;
    std::vector<int> address(L2), payment(L2);
    for (int i = 0; i < P; ++i) {
        int r, c, p;
        std::cin >> r >> c >> p;
        const int pos = (r - 1) * L + c - 1;
        address[pos] = 1 << i;
        payment[pos] = p;
    }

    const int FULL = 1 << P;
    const int64_t INF = 1e18;
    std::vector<std::vector<int64_t>> dp(L2, std::vector<int64_t>(FULL, -INF)); // {pos, set} -> score
    dp[(R - 1) * L + C - 1][0] = 0; // start position, empty set
    for (int z = M; z; --z) {
        std::vector<std::vector<int64_t>> ndp(L2, std::vector<int64_t>(FULL, -INF));
        for (int pos = 0; pos < L2; ++pos)
            for (int m = 0; m < FULL; ++m) {
                if (dp[pos][m] == -INF) // unreachable
                    continue;
                ndp[pos][m] = std::max(ndp[pos][m], dp[pos][m]);
                const int r = pos / L, c = pos % L;
                for (int dir = 0; dir < 4; ++dir) {
                    const int nr = r + DR[dir];
                    const int nc = c + DC[dir];
                    if (~nr && ~nc && nr < L && nc < L) {
                        const auto npos = nr * L + nc;
                        const auto ns = calculate_toll(dp[pos][m], tolls[dir]);
                        ndp[npos][m] = std::max(ndp[npos][m], ns);

                        const int nm = m | address[npos];
                        const int npay = (m & address[npos]) ? 0 : payment[npos];
                        ndp[npos][nm] = std::max(ndp[npos][nm], ns + npay);
                    }
                }
            }
        dp = std::move(ndp);
    }

    int64_t ans = -INF;
    for (int pos = 0; pos < L2; ++pos)
        ans = std::max(ans, dp[pos][FULL - 1]);
    if (ans == -INF)
        std::cout << "IMPOSSIBLE";
    else
        std::cout << ans;
}

static void solve_set1() {
    int L, P, M, R, C;
    std::cin >> L >> P >> M >> R >> C;
    std::array<toll_t, 4> tolls;
    for (auto& [op, fac] : tolls)
        std::cin >> op >> fac;
    assert(P == 0);

    const int L2 = L * L;
    std::vector<int64_t> score(L2, -1e18), time(L2);
    std::queue<std::array<int64_t, 4>> qq; // {after moves, with score, row, col}
    qq.push({0, 0, --R, --C});
    while (!qq.empty()) {
        const auto [t, s, r, c] = qq.front();
        qq.pop();
        if (t == M)
            continue;

        if (0 <= r - 1) { // N
            const auto pos = (r - 1) * L + c;
            const auto ns = calculate_toll(s, tolls[N]);
            if (score[pos] < ns || (score[pos] == ns && t + 1 < time[pos])) {
                score[pos] = ns;
                time[pos] = t + 1;
                qq.push({t + 1, ns, r - 1, c});
            }
        }
        if (c + 1 < L) { // E
            const auto pos = r * L + c + 1;
            const auto ns = calculate_toll(s, tolls[E]);
            if (score[pos] < ns || (score[pos] == ns && t + 1 < time[pos])) {
                score[pos] = ns;
                time[pos] = t + 1;
                qq.push({t + 1, ns, r, c + 1});
            }
        }
        if (0 <= c - 1) { // W
            const auto pos = r * L + c - 1;
            const auto ns = calculate_toll(s, tolls[W]);
            if (score[pos] < ns || (score[pos] == ns && t + 1 < time[pos])) {
                score[pos] = ns;
                time[pos] = t + 1;
                qq.push({t + 1, ns, r, c - 1});
            }
        }
        if (r + 1 < L) { // S
            const auto pos = (r + 1) * L + c;
            const auto ns = calculate_toll(s, tolls[S]);
            if (score[pos] < ns || (score[pos] == ns && t + 1 < time[pos])) {
                score[pos] = ns;
                time[pos] = t + 1;
                qq.push({t + 1, ns, r + 1, c});
            }
        }
    }

    const int64_t ans = *std::max_element(score.begin(), score.end());
    std::cout << std::max(ans, int64_t{});
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 pizza_delivery.cpp -o pizza_delivery.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address pizza_delivery.cpp -o pizza_delivery

Run:
pizza_delivery.exe < pizza_delivery.in

Input:

2
3 0 1 1 2
+ 1
- 2
+ 3
/ 4
3 0 1 2 3
- 2
- 2
- 2
- 2

Output:

Case #1: 3
Case #2: 0

*/

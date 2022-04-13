
#include <iostream>
#include <numeric>
#include <vector>

// Weightlifting
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000877ba5/0000000000aa9280

constexpr const int INF = 1e09;
std::vector<std::vector<int>> commons;
std::vector<std::vector<std::vector<int>>> memo;

static void solve() { // inspired by tourist and ksun48
    int R, C;
    std::cin >> R >> C;

    std::vector<std::vector<int>> weights(R, std::vector<int>(C));
    for (auto& row : weights)
        for (auto& w : row)
            std::cin >> w;

    commons.assign(R, std::vector<int>(R));
    for (int r = 0; r < R; ++r) {
        auto base = weights[r];
        commons[r][r] = std::accumulate(base.begin(), base.end(), 0);
        for (int s = r + 1; s < R; ++s) {
            for (int e = 0; e < C; ++e)
                base[e] = std::min(base[e], weights[s][e]);
            commons[r][s] = std::accumulate(base.begin(), base.end(), 0);
        }
    }

    std::vector<std::vector<int>> dp(R, std::vector<int>(R));
    for (int u = 0; u < R; ++u)
        for (int l = u; ~l; --l) {
            auto& cur = dp[l][u];
            if (l == u)
                cur = commons[l][u];
            else {
                cur = INF;
                const int range = u - l;
                for (int len = 0; len < range ; ++len)
                    cur = std::min(cur, dp[l][l + len] + dp[l + len + 1][u]);
                cur -= commons[l][u];
            }
        }

    std::cout << dp[0][R - 1] * 2;
}

static void recommended() {
    int R, C;
    std::cin >> R >> C;

    std::vector<std::vector<int>> weights(R, std::vector<int>(C));
    for (auto& row : weights)
        for (auto& w : row)
            std::cin >> w;

    commons.assign(R, std::vector<int>(R));
    for (int r = 0; r < R; ++r) {
        auto base = weights[r];
        commons[r][r] = std::accumulate(base.begin(), base.end(), 0);
        for (int s = r + 1; s < R; ++s) {
            for (int e = 0; e < C; ++e)
                base[e] = std::min(base[e], weights[s][e]);
            commons[r][s] = std::accumulate(base.begin(), base.end(), 0);
        }
    }

    std::vector<std::vector<int>> dp(R, std::vector<int>(R));
    for (int u = 0; u < R; ++u)
        for (int l = u; ~l; --l) {
            auto& cur = dp[l][u];
            if (l == u)
                cur = 0;
            else {
                cur = INF;
                const int range = u - l;
                for (int len = 0; len < range ; ++len)
                    cur = std::min(cur, dp[l][l + len] + commons[l][l + len] - commons[l][u] + dp[l + len + 1][u] + commons[l + len + 1][u] - commons[l][u]);
            }
        }

    std::cout << (dp[0][R - 1] + commons[0][R - 1]) * 2;
}

static int divandconq(const int begin, const int end, const int bottom) {
    const int cm = commons[begin][end];
    if (begin == end)
        return 2 * (cm - bottom);
    auto& m = memo[begin][end][bottom];
    if (m > 0)
        return m;

    int ans = INF;
    const int range = end - begin;
    for (int len = 0; len < range ; ++len)
        ans = std::min(ans, divandconq(begin, begin + len, cm) + divandconq(begin + len + 1, end, cm));

    return m = ans + 2 * (cm - bottom);
}

/**
 * This solution is accepted for Set2 but it is too complicated and memory demanding
 */
static void solve_bigmem() {
    int R, C;
    std::cin >> R >> C;

    std::vector<std::vector<int>> weights(R, std::vector<int>(C));
    for (auto& row : weights)
        for (auto& w : row)
            std::cin >> w;

    int haviest{0};
    commons.assign(R, std::vector<int>(R));
    for (int r = 0; r < R; ++r) {
        auto base = weights[r];
        commons[r][r] = std::accumulate(base.begin(), base.end(), 0);
        haviest = std::max(haviest, commons[r][r]);
        for (int s = r + 1; s < R; ++s) {
            for (int e = 0; e < C; ++e)
                base[e] = std::min(base[e], weights[s][e]);
            commons[r][s] = std::accumulate(base.begin(), base.end(), 0);
        }
    }

    memo.assign(R, std::vector<std::vector<int>>(R, std::vector<int>(haviest + 1)));
    std::cout << divandconq(0, R - 1, 0);
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
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 weightlifting.cpp -o weightlifting.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address weightlifting.cpp -o weightlifting

Run:
weightlifting.exe < weightlifting.in

Input:

3
3 1
1
2
1
2 3
1 2 1
2 1 2
3 3
3 1 1
3 3 3
2 3 3

Output:


*/

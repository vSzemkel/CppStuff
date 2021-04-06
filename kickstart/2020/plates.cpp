
#include <algorithm>
#include <numeric>
#include <iostream>
#include <unordered_map>
#include <vector>

// Plates
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ffc7/00000000001d40bb#problem

int g_N, g_K, g_P;
std::unordered_map<int, int> g_cache;
std::vector<std::vector<int>> g_plates;

static int nice() // O(N*P*K)
{
    const int max_take = std::min(g_P, g_K);
    std::vector<std::vector<int>> dp(g_N + 1, std::vector<int>(g_P + 1));
    for (int i = 1; i <= g_N; ++i) {
        dp[i] = dp[i - 1];
        for (int j = 1; j <= g_P; ++j)
            for (int t = 1; t <= std::min(j, max_take); ++t)
                dp[i][j] = std::max(dp[i][j], dp[i - 1][j - t] + g_plates[i - 1][t - 1]);
    }

    return dp[g_N][g_P];
}

static int ugly(const int count, const int prefix)
{
    if (count == 0 || prefix == g_N)
        return 0;
    const int hash = (count << 16) + prefix;
    const auto cached = g_cache.find(hash);
    if (cached != g_cache.end())
        return cached->second;

    const int max_take = std::min(count, g_K);
    std::vector<int> candidates(max_take + 1);
    candidates[0] = ugly(count, prefix + 1);
    for (int i = 1; i <= max_take; ++i)
        candidates[i] = g_plates[prefix][i - 1] + ugly(count - i, prefix + 1);

    const auto it = std::max_element(candidates.begin(), candidates.end());
    g_cache[hash] = *it;
    return *it;
}

static void solve() {
    std::cin >> g_N >> g_K >> g_P;
    g_plates.assign(g_N, std::vector<int>(g_K));
    for (int i = 0; i < g_N; ++i) {
        for (int j = 0; j < g_K; ++j)
            std::cin >> g_plates[i][j];
        std::inclusive_scan(g_plates[i].begin(), g_plates[i].end(), g_plates[i].begin());
        /*int sum{0};
        for (auto& b : g_plates[i]) {
            sum += b;
            b = sum;
        }*/
    }

    //g_cache.clear();
    //std::cout << ugly(g_P, 0);
    std::cout << nice();
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 g_plates.cpp -o g_plates.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address g_plates.cpp -o g_plates.o

Run:
py.exe interactive_runner.py py.exe g_plates_testing_tool.py 1 -- g_plates.exe
g_plates.exe < g_plates.in

Input:

2
2 4 5
10 10 100 30
80 50 10 50
3 2 3
80 80
15 50
20 1

Output:

Case #1: 250
Case #2: 180

*/

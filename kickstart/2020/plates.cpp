
#include <algorithm>
#include <numeric>
#include <iostream>
#include <unordered_map>
#include <vector>

// Plates
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ffc7/00000000001d40bb#problem

int g_N;
std::unordered_map<int, int> g_cache;
std::vector<std::vector<int>> g_plates;

static int dp(const int count, const int prefix)
{
    if (count == 0 || prefix == g_N)
        return 0;
    const int hash = (count << 16) + prefix;
    const auto cached = g_cache.find(hash);
    if (cached != g_cache.end())
        return cached->second;

    const int max_take = std::min(count, (int)g_plates[prefix].size());
    std::vector<int> candidates(max_take + 1);
    candidates[0] = dp(count, prefix + 1);
    for (int i = 1; i <= max_take; ++i)
        candidates[i] = g_plates[prefix][i - 1] + dp(count - i, prefix + 1);

    const auto it = std::max_element(candidates.begin(), candidates.end());
    g_cache[hash] = *it;
    return *it;
}

static void solve() {
    int K, P; std::cin >> g_N >> K >> P;
    g_plates.assign(g_N, std::vector<int>(K));
    for (int i = 0; i < g_N; ++i) {
        for (int j = 0; j < K; ++j)
            std::cin >> g_plates[i][j];
        std::inclusive_scan(g_plates[i].begin(), g_plates[i].end(), g_plates[i].begin());
    }

    g_cache.clear();
    std::cout << dp(P, 0);
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


*/


#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

// Trapping Rain Water
// https://leetcode.com/problems/trapping-rain-water/?envType=study-plan-v2&envId=top-interview-150

constexpr int INF = 1e09;

static int64_t solve() { // O(N) S(1)
    int N;
    std::cin >> N;
    std::vector<int> bars(N);
    for (auto& h : bars)
        std::cin >> h;

    if (N < 3)
        return 0;

    int64_t ans{};
    int maxl{}, maxr{}, left{}, right{N - 1};
    while (left <= right)
        if (maxl < maxr) {
            if (bars[left] < maxl)
                ans += maxl - bars[left];
            maxl = std::max(maxl, bars[left]);
            ++left;
        } else {
            if (bars[right] < maxr)
                ans += maxr - bars[right];
            maxr = std::max(maxr, bars[right]);
            --right;
        }

    return ans;
}

static int64_t solve_greedy() { // O(N) S(N)
    int N;
    std::cin >> N;
    std::vector<int> bars(N);
    for (auto& h : bars)
        std::cin >> h;

    if (N < 3)
        return 0;

    int highest = 0;
    std::vector<int> traps(N, INF);
    for (int i = 0; i < N; ++i) {
        traps[i] = std::min(traps[i], std::max(highest - bars[i], 0));
        highest = std::max(highest, bars[i]);
    }

    highest = 0;
    for (int i = N - 1; ~i; --i) {
        traps[i] = std::min(traps[i], std::max(highest - bars[i], 0));
        highest = std::max(highest, bars[i]);
    }

    return std::accumulate(traps.begin(), traps.end(), int64_t{});
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": " << solve() << '\n';
    }
}

/*

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 trapping_rain_water.cpp -o trapping_rain_water.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address trapping_rain_water.cpp -o trapping_rain_water

Run:
trapping_rain_water.exe < trapping_rain_water.in

Input:

1
12
0 1 0 2 1 0 1 3 2 1 2 1

Output:


*/

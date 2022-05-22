
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

// Energy Stones
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050eda/00000000001198c3

constexpr const int MAXT = 100000;

/**
 * @brief Not all stones will be efectvely eaten, Eat next or do not - DP problem
 * Tricky part is with the observation: optimal set has optimal order
 * For Set1 eat fastest decaying stones first: D1 > D2
 * For Set2 eat (shortest eaten/fastest decaying) E1/D1 < E2/D2
 */
static void solve() {
    int N; std::cin >> N;
    std::vector<int> energy(N), decay(N), eattime(N), order(N);
    for (int i = 0; i < N; ++i)
        std::cin >> eattime[i] >> energy[i] >> decay[i];

    std::iota(order.begin(), order.end(), 0);
    std::sort(order.begin(), order.end(), [&](const int o1, const int o2) {
        return eattime[o1] * decay[o2] < eattime[o2] * decay[o1];
    });

    int time{0}; // time elapsed for case of eating all the stones [0..s-1]
    std::vector<int64_t> dp(MAXT, 0); // scores per time of finishing eating last choosen stone
    for (int s : order) {
        for (int64_t t = time; ~t; --t) {
            auto& cur = dp[t + eattime[s]];
            cur = std::max(cur, dp[t] + std::max(int64_t{}, energy[s] - decay[s] * t));
        }
        time += eattime[s];
    }

    std::cout << *std::max_element(dp.begin(), dp.end());
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
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 energy_stones.cpp -o energy_stones.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address energy_stones.cpp -o energy_stones

Run:
energy_stones.exe < energy_stones.in

Input:

4
3
10 4 1000
10 3 1000
10 8 1000
2
10 2 0
10 3 0
4
20 10 1
5 30 5
100 30 1
5 80 60
2
12 300 50
5 200 0

Output:

Case #1: 8
Case #2: 5
Case #3: 105
Case #4: 500

*/


#include <array>
#include <iostream>
#include <utility>
#include <vector>

// Knapsack problem 
// Givent N items of price and value, pack a limited capacity knapsack maximizing its value

constexpr const int N = 12;
constexpr const int MAXCAP = 100;
std::array<int, N> weights = {35, 12, 9, 20, 14, 7, 7, 10, 4, 18, 12, 14};
std::array<int, N> values = {3, 6, 1, 7, 2, 5, 8, 9, 2, 4, 2, 6};

static auto knapsack() {
    // dp[i][j] optimal value of first i items not exceeding capacity j
    std::vector<std::vector<int>> dp(N + 1, std::vector<int>(MAXCAP + 1));
    std::vector<std::vector<bool>> choosen(N + 1, std::vector<bool>(MAXCAP + 1));
    for (int i = 0; i < N; ++i)
        for (int cap = 0; cap <= MAXCAP; ++cap)
            if (weights[i] <= cap && dp[i][cap] < dp[i][cap - weights[i]] + values[i]) { // allowed && reasonable
                dp[i + 1][cap] = dp[i][cap - weights[i]] + values[i];
                choosen[i + 1][cap] = true;
            } else
                dp[i + 1][cap] = dp[i][cap];

    int sum{0}, val{0};
    std::vector<int> ans;
    for (int i = N; i; --i)
        if (choosen[i][MAXCAP - sum]) {
            const int ind = i - 1;
            ans.push_back(ind);
            sum += weights[ind];
            val += values[ind];
            std::cout << "Taken: " << i << " of weight " << weights[ind] << " and value " << values[ind] << '\n';
            std::cout << "Current weight: " << sum << ", current value: " << val << '\n';
        }

    return std::pair{std::pair{dp[N][MAXCAP], sum}, ans};
}

int main(int, char**)
{
    const auto k = knapsack();
    std::cout << "Taken " << k.second.size() << " items of total value " << k.first.first << " and weight " << k.first.second << " can be packed\n";
    for (int i = int(k.second.size()) - 1; i >= 0; --i) std::cout << k.second[i] << ' ';
    std::cout << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 knapsack.cpp -o knapsack.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address knapsack.cpp -o knapsack

*/

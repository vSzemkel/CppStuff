
#include <iostream>
#include <vector>

// Rod cutting
// Given a steel rod of a length N and sell prices for
// rods of lengths [1..N] plan the optimal cutting


int main(int, char**)
{
    const int INF = 1e09;
    int N; std::cin >> N;
    std::vector<int> prices(N + 1);
    for (int i = 1; i <= N; ++i)
        std::cin >> prices[i];

    std::vector<int> dp(N + 1, -INF); // dp[i] optimal price for i-rod
    std::vector<int> memo(N + 1);     // memo[i] first optimal cut length of i-rod
    dp[0] = 0;
    for (int i = 1; i <= N; ++i) {
        auto& cur = dp[i];
        for (int j = 1; j <= i; ++j) {
            const auto can = prices[j] + dp[i - j];
            if (cur < can) {
                cur = can;
                memo[i] = j;
            }
        }
    }

    std::cout << "With cuts: ";
    for (int c = N; c; c -= memo[c])
        std::cout << memo[c] << ' ';
    std::cout << "the optimal value is " << dp[N] << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 rod_cutting.cpp -o rod_cutting.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address rod_cutting.cpp -o rod_cutting

Run:
rod_cutting.exe < rod_cutting.in

Input:

10
1 4 8 7 5 17 4 6 8 10

Output:

With cuts: 1 3 6 the optimal value is 26

*/

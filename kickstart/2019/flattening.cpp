
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

// Flattening
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050edc/000000000018666c

/**
 * @Observation1: In each equal height section we just eliminate non dominant numbers
 * @Observation2: If two adjacent sections will have equal height, we cover this case just with smaller number of sections
 */
static void solve() {
    int N, K;
    std::cin >> N >> K;
    std::vector<int> data(N);
    for (auto& n : data)
        std::cin >> n;

    // compute cost of making range [i..j] uniform height
    std::vector<std::vector<int>> uniform_height_cost(N, std::vector<int>(N));
    for (int i = 0; i < N; ++i) {
        int mx{0};
        std::unordered_map<int, int> count;
        for (int j = i; j < N; ++j) {
            auto& cur = count[data[j]];
            ++cur;
            mx = std::max(mx, cur);
            uniform_height_cost[i][j] = j - i + 1 - mx;
        }
    }

    // dp[i][j] - minimal cost of making range [1..i] not exceeding j steps
    std::vector<std::vector<int>> dp(N + 1, std::vector<int>(K + 1, 1000));
    std::fill_n(dp[0].begin(), K + 1, 0);
    for (int i = 1; i <= N; ++i) {
        dp[i][0] = uniform_height_cost[0][i - 1];
        for (int j = 1; j <= K; ++j)
            for (int k = i; k > 0; --k)
                dp[i][j] = std::min(dp[i][j], dp[k - 1][j - 1] + uniform_height_cost[k - 1][i - 1]);
    }

    std::cout << dp[N][K];
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
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++17 flattening.cpp -o flattening.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address flattening.cpp -o flattening

Run:
flattening.exe < flattening.in

Input:

4
8 2
300 100 300 300 200 100 800 500
5 3
100 100 100 100 3
7 3
10 20 40 10 10 30 30
10 2
30 30 60 60 90 90 60 60 30 30

Output:

Case #1: 3
Case #2: 0
Case #3: 1
Case #4: 2

*/

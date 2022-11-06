
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

// Catch Some
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050ff2/0000000000150a0d

static void solve() {
    int N, K;
    std::cin >> N >> K;
    std::vector<int> positions(N), colors(N);
    for (auto& p : positions)
        std::cin >> p;
    for (auto& c : colors)
        std::cin >> c;

    std::unordered_map<int, std::vector<int>> world; // color -> positions
    for (int i = 0; i < N; ++i)
        world[colors[i]].push_back(positions[i]);
    for (auto& [c, p] : world)
        std::sort(p.begin(), p.end());
    const auto C = world.size();

    // [i][j][b] - from first i colors take j dogs. b == one of 1..i colors is the last one
    std::vector<std::vector<std::vector<int64_t>>> dp(C + 1, std::vector<std::vector<int64_t>>(K + 1, std::vector<int64_t>(2, 1e18)));
    for (size_t c = 0; c <= C; ++c) // taking 0 dogs from any state is free
        dp[c][0][0] = dp[c][0][1] = 0;

    int count{1};
    for (const auto& [c, p] : world) { // take i from [1..count-1] colors and j from count-th color
        const int size = int(p.size());
        for (int i = 0; i <= K; ++i) { 
            auto& prev =  dp[count - 1][i];
            for (int j = 0; j <= size; ++j) {
                if (i + j > K) break;
                auto& cur = dp[count][i + j];
                const int dist = j ? p[j - 1] : 0;
                cur[0] = std::min(cur[0], prev[0] + 2 * dist);
                cur[1] = std::min(cur[1], prev[1] + 2 * dist);
                cur[1] = std::min(cur[1], prev[0] + dist);
            }
        }
        ++count;
    }

    std::cout << dp[C][K][1];
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 catch_some.cpp -o catch_some.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address catch_some.cpp -o catch_some

Run:
catch_some.exe < catch_some.in

Input:

3
4 3
1 2 4 9
3 3 2 3
4 3
1 2 3 4
1 8 1 8
6 6
4 3 3 1 3 10000
1 2 8 9 5 7

Output:

Case #1: 8
Case #2: 6
Case #3: 10028

*/

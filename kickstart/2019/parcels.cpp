
#include <array>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

// Parcels
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050e01/000000000006987d

constexpr const int INF = 1e09;

static void solve() {
    int R, C;
    std::cin >> R >> C;
    std::vector<std::string> board(R);
    for (auto& r : board)
        std::cin >> r;

    // for each cell compute distance to closest office
    std::queue<std::pair<std::array<int, 2>, int>> qq; // {target, distance}
    std::vector<std::vector<bool>> seen(R, std::vector<bool>(C));
    std::vector<std::vector<int>> distances(R, std::vector<int>(C, INF));
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c)
            if (board[r][c] == '1')
                qq.push({{r, c}, 0});
    int max{0};
    while (!qq.empty()) {
        auto [cell, cost] = qq.front(); qq.pop();
        const auto& [r, c] = cell;
        if (!seen[r][c]) {
            seen[r][c] = true;
            distances[r][c] = cost;
            max = std::max(max, cost);
            if (0 < c && !seen[r][c - 1]) qq.push({{r, c - 1}, cost + 1});
            if (c < C - 1 && !seen[r][c + 1]) qq.push({{r, c + 1}, cost + 1});
            if (0 < r && !seen[r - 1][c]) qq.push({{r - 1, c}, cost + 1});
            if (r < R - 1 && !seen[r + 1][c]) qq.push({{r + 1, c}, cost + 1});
        }
    }

    int ans = INF;
    for (int s = 0; s < R; ++s)
        for (int d = 0; d < C; ++d)
            if (distances[s][d] >= 3 * max / 5) { // new office not to close old ones
                int can = 0;
                for (int r = 0; r < R; ++r)
                    for (int c = 0; c < C; ++c) {
                        const int dist = std::min(distances[r][c], std::abs(r - s) + std::abs(c - d));
                        can = std::max(can, dist);
                    }
                ans = std::min(ans, can);
            }

    std::cout << (ans == INF ? 0 : ans);
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
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 parcels.cpp -o parcels.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address parcels.cpp -o parcels

Run:
py.exe interactive_runner.py py.exe parcels_testing_tool.py 1 -- parcels.exe
parcels.exe < parcels.in

Input:

3
3 3
101
000
101
1 2
11
5 5
10001
00000
00000
00000
10001

Output:

Case #1: 1
Case #2: 0
Case #3: 2

*/

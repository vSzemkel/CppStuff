
#include <array>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

// Parcels
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050e01/000000000006987d
// tag: Manhattan distance

constexpr const int INF = 1e09;

template <typename T, typename U>
T first_true(T lo, T hi, U f) {
    hi++;
    while (lo < hi) { // find first index such that f is true
        const T mid = lo + (hi - lo) / 2; // this will work for negative numbers too
        f(mid) ? hi = mid : lo = mid + 1;
    }
    return lo;
}

static void solve() {
    int R, C;
    std::cin >> R >> C;
    std::vector<std::string> board(R);
    for (auto& r : board)
        std::cin >> r;

    // for each cell compute distance to closest office
    std::queue<std::pair<std::array<int, 2>, int>> qq; // {target, distance}
    std::vector<std::vector<int>> distances(R, std::vector<int>(C, INF));
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c)
            if (board[r][c] == '1')
                qq.push({{r, c}, 0});
    int max{0};
    while (!qq.empty()) {
        auto [cell, cost] = qq.front(); qq.pop();
        const auto& [r, c] = cell;
        if (distances[r][c] == INF) {
            distances[r][c] = cost;
            max = std::max(max, cost);
            ++cost;
            if (0 < c && distances[r][c - 1] == INF) qq.push({{r, c - 1}, cost});
            if (c < C - 1 && distances[r][c + 1] == INF) qq.push({{r, c + 1}, cost});
            if (0 < r && distances[r - 1][c] == INF) qq.push({{r - 1, c}, cost});
            if (r < R - 1 && distances[r + 1][c] == INF) qq.push({{r + 1, c}, cost});
        }
    }

    /**
     * @brief Checks if with one additional office enables
     * delivery with max ditance not exceeding d
     */
    const auto check = [&](const int d) -> bool {
        // compute region such that placing an office in it enable delivering
        // distance <= d for each point reachable with distance >d from original offices
        bool found{};
        int ne = INF, nw = INF, sw = -INF, se = -INF; // manhattan wheel edges
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                if (distances[r][c] > d) {
                    ne = std::min(ne, r + c + d);
                    nw = std::min(nw, r - c + d);
                    sw = std::max(sw, r + c - d);
                    se = std::max(se, r - c - d);
                    found = true;
                }

        if (!found)
            return true;

        if (sw <= ne && se <= nw)
            for (int r = 0; r < R; ++r)
                for (int c = 0; c < C; ++c) {
                    const int normal = r + c, mirrored = r - c;
                    if (sw <= normal && normal <= ne && se <= mirrored && mirrored <= nw)
                        return true;
                }

        return false;
    };

    std::cout << first_true(0, max, check);
}

static void solve_set1() {
    int R, C;
    std::cin >> R >> C;
    std::vector<std::string> board(R);
    for (auto& r : board)
        std::cin >> r;

    // for each cell compute distance to closest office
    std::queue<std::pair<std::array<int, 2>, int>> qq; // {target, distance}
    std::vector<std::vector<int>> distances(R, std::vector<int>(C, INF));
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c)
            if (board[r][c] == '1')
                qq.push({{r, c}, 0});
    int max{0};
    while (!qq.empty()) {
        auto [cell, cost] = qq.front(); qq.pop();
        const auto& [r, c] = cell;
        if (distances[r][c] == INF) {
            distances[r][c] = cost;
            max = std::max(max, cost);
            if (0 < c && distances[r][c - 1] == INF) qq.push({{r, c - 1}, cost + 1});
            if (c < C - 1 && distances[r][c + 1] == INF) qq.push({{r, c + 1}, cost + 1});
            if (0 < r && distances[r - 1][c] == INF) qq.push({{r - 1, c}, cost + 1});
            if (r < R - 1 && distances[r + 1][c] == INF) qq.push({{r + 1, c}, cost + 1});
        }
    }

    int ans = INF;
    for (int s = 0; s < R; ++s)
        for (int d = 0; d < C; ++d)
            if (distances[s][d] > 0) { // new office not to close old ones
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

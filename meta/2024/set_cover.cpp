
#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <vector>

// Set Cover
// https://www.facebook.com/codingcompetitions/hacker-cup/2024/round-3/problems/A

constinit const int INF = 1e09;
int N, K, ans;
std::vector<std::string> board;

template <typename T> void setmax(T& a, T b) { if (a < b) a = b; }
template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

static void check(int height, int width) {
    if (height > 0 && width > 0)
        setmax(ans, (height + 1) * (width + 1));
}

static int solve() {
    std::cin >> N >> K;
    board.resize(N);
    for (auto& r : board)
        std::cin >> r;
    ans = K;

    int r{INF}, R{-1}, c{INF}, C{-1}, rq{INF}, Rq{-1}, cq{INF}, Cq{-1};
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            const auto& m = board[i][j];
            if (m == '1') {
                setmin(r, i);
                setmax(R, i);
                setmin(c, j);
                setmax(C, j);
            } else if (m == '?') {
                setmin(rq, i);
                setmax(Rq, i);
                setmin(cq, j);
                setmax(Cq, j);
            }
        }

    switch (K) {
        case 0:
            check(R - r, C - c);
            break;
        case 1:
            for (int i = 0; i < N; ++i)
                for (int j = 0; j < N; ++j)
                    if (board[i][j] == '?') {
                        check(R - i, C - j);
                        check(i - r, C - j);
                        check(i - r, j - c);
                        check(R - i, j - c);
                    }
            break;
        case 2: {
            std::vector<int> min_qcol_in_row(N, INF);
            std::vector<int> max_qcol_in_row(N, -INF);
            for (int i = 0; i < N; ++i)
                for (int j = 0; j < N; ++j)
                    if (board[i][j] == '?') {
                        setmin(min_qcol_in_row[i], j);
                        setmax(max_qcol_in_row[i], j);
                    }

            for (int top = 0; top < N; ++top)
                for (int bottom = top; bottom < N; ++bottom)
                    if (0 <= max_qcol_in_row[top] && 0 <= max_qcol_in_row[bottom]) {
                        const auto height = std::max(R, bottom) - std::min(r, top);
                        check(height, std::max(C, max_qcol_in_row[top]) - std::min(c, min_qcol_in_row[bottom]));
                        check(height, std::max(C, max_qcol_in_row[bottom]) - std::min(c, min_qcol_in_row[top]));
                    }
            break;
        }
        case 3: {
            const int rr = std::min(r, rq);
            const int RR = std::max(R, Rq);
            const int cc = std::min(c, cq);
            const int CC = std::max(C, Cq);
            for (int i = 0; i < N; ++i)
                for (int j = 0; j < N; ++j)
                    if (board[i][j] == '?') { // try it as a corner
                        check(RR - std::min(i, rr), CC - std::min(j, cc));
                        check(std::max(i, RR) - rr, CC - std::min(j, cc));
                        check(RR - std::min(i, rr), std::max(j, CC) - cc);
                        check(std::max(i, RR) - rr, std::max(j, CC) - cc);
                    }
            break;
        }
        default:
            check(std::max(R, Rq) - std::min(r, rq), std::max(C, Cq) - std::min(c, cq));
            break;
    }

    return ans;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << std::format("Case #{}: {}\n", g, solve());
    }
}

/*

Format:
clang-format -i set_cover.cpp

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 set_cover.cpp
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address set_cover.cpp
clang -Wall -Wextra -Og -ggdb3 -lstdc++ -std=c++20 set_cover.cpp

Run:
py.exe interactive_runner.py py.exe set_cover_testing_tool.py 1 -- a.exe
a.exe < set_cover.in

Input:


Output:


*/

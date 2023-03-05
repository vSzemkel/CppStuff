
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

// Diagonal Puzzle
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050edd/00000000001a2835

constexpr const int INF = 1e09;
constexpr const int SPIN = 4;
constexpr const int SLASHED = 2;
constexpr const int BACK_SLASHED = 1;

static void solve() {
    int N;
    std::cin >> N;
    const int par = N & 1;
    const std::string pat(N, '#');
    std::vector<std::string> board(N);
    for (auto& r : board)
        std::cin >> r;

    int ans{INF};
    const auto turn = [](char& c){ c = (c == '.' ? '#' : '.'); };
    for (int m = 0; m < (1 << (2 + par)); ++m) {
        int cur{0};
        auto copy = board;
        if (m & SPIN)
            for (int t = 0, b = N - 1; t < b; ++t, --b)
                std::swap(copy[t], copy[b]);
        if (m & SLASHED) {
            ++cur;
            for (int z = N, r = N - 1, c = 0; z; --z, --r, ++c)
                turn(copy[r][c]);
        }
        if (m & BACK_SLASHED) {
            ++cur;
            for (int z = N - par, r = 0, c = par; z; --z, ++r, ++c)
                turn(copy[r][c]);
        }

        for (int r = N - 1, c = 0; c <= r; --r, ++c)
            if (copy[r][c] == '.') {
                ++cur;
                for (int y = 2 * c + 1, s = N - y, d = 0; y; --y, ++s, ++d)
                    turn(copy[s][d]);
            }
        for (int r = 0, c = N - 1; r < c; ++r, --c)
            if (copy[r][c] == '.') {
                ++cur;
                for (int y = 2 * (N - c) - 1, s = 0, d = N - y; y; --y, ++s, ++d)
                    turn(copy[s][d]);
            }

        for (int r = 0, c = par; r < N / 2; ++r, ++c)
            if (copy[r][c] == '.') {
                ++cur;
                for (int y = 2 * r + 1 + par, s = 2 * r + par, d = 0; y; --y, --s, ++d)
                    turn(copy[s][d]);
            }
        for (int r = N - 1 - par, c = N - 1; r >= N / 2; --r, --c)
            if (copy[r][c] == '.') {
                ++cur;
                for (int y = N - r, s = N - 1, d = r; y; --y, --s, ++d)
                    turn(copy[s][d]);
            }

        if (std::all_of(copy.begin(), copy.end(), [&pat](auto& r){ return r == pat; }))
            ans = std::min(ans, cur);
    }
    std::cout << ans;
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
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++17 diagonal_puzzle.cpp -o diagonal_puzzle.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address diagonal_puzzle.cpp -o diagonal_puzzle

Run:
py.exe interactive_runner.py py.exe diagonal_puzzle_testing_tool.py 1 -- diagonal_puzzle.exe
diagonal_puzzle.exe < diagonal_puzzle.in

Input:

3
3
..#
#.#
#..
5
.####
#.###
##.##
###.#
#####
2
##
##

Output:


*/

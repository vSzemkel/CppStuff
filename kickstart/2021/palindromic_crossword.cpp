
#include <array>
#include <iostream>
#include <string>
#include <vector>

// Palindromic crossword
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000043585c/0000000000859dcd

int R, C;
std::vector<std::string> board;
constexpr int N{0}, E{1}, S{2}, W{3};
std::array<std::vector<int>, 4> wall;
static int drill_v(const int r, const int c, const int val);

const auto row_peer_off = [](const int cell) { return wall[S][cell] - wall[N][cell]; };
const auto col_peer_off = [](const int cell) { return wall[E][cell] - wall[W][cell]; };

static int drill_h(const int r, const int c, const int val) {
    const int next_c = c + col_peer_off(r * C + c);
    auto& next = board[r][next_c];
    if (next == '.') {
        next = val;
        return 1 + drill_v(r, next_c, val);
    }

    return 0;
}

static int drill_v(const int r, const int c, const int val) {
    const int next_r = r + row_peer_off(r * C + c);
    auto& next = board[next_r][c];
    if (next == '.') {
        next = val;
        return 1 + drill_h(next_r, c, val);
    }

    return 0;
}

static void solve() {
    std::cin >> R >> C;
    board.resize(R);
    for (auto& r : board) std::cin >> r;

    const int size = R * C;
    wall.fill(std::vector<int>(size, 1));
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c) {
            const auto cell = r * C + c;
            if (board[r][c] == '#') {
                wall[N][cell] = wall[W][cell] = wall[S][cell] = wall[E][cell] = 0;
                continue;
            }
            if (c > 0)
                wall[W][cell] = wall[W][cell - 1] + 1;
            if (r > 0)
                wall[N][cell] = wall[N][cell - C] + 1;
        }
    for (int r = R - 1; r >= 0; --r)
        for (int c = C - 1; c >= 0; --c) {
            const auto cell = r * C + c;
            if (board[r][c] == '#')
                continue;
            if (c < C - 1)
                wall[E][cell] = wall[E][cell + 1] + 1;
            if (r < R - 1)
                wall[S][cell] = wall[S][cell + C] + 1;
        }

    int ret{0};
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c) {
            const auto val = board[r][c];
            if (val != '.' && val != '#') {
                ret += drill_h(r, c, val);
                ret += drill_v(r, c, val);
            }
        }

    std::cout << ret << '\n';
    for (int r = 0; r < R; ++r)
        std::cout << board[r] << '\n';
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve();
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 palindromic_crossword.cpp -o palindromic_crossword.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address palindromic_crossword.cpp -o palindromic_crossword

Run:
palindromic_crossword.exe < palindromic_crossword.in

Input:

2
2 2
A.
.#
4 6
A...#.
B##...
B.###.
A...#.

Output:

Case #1: 2
AA
A#
Case #2: 8
A..A#.
B##A.A
BB###A
ABBA#.

*/

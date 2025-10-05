
#include <algorithm>
#include <array>
#include <iostream>
#include <format>
#include <unordered_map>

// Four in a Burrow
// https://www.facebook.com/codingcompetitions/hacker-cup/2024/round-2/problems/B


static constexpr int WINS = 4;
static constexpr int ROWS = 6;
static constexpr int COLS = 7;
static constexpr int SIZE = ROWS * COLS;
static constexpr int DR[] = {1, 1, 1, 0, -1, -1, -1, 0};
static constexpr int DC[] = {1, 0, -1, -1, -1, 0, 1, 1};

bool cwon, fwon;
std::array<int, COLS> column_height;
std::array<std::string, ROWS> board;

// Observation1: Order of wins by column is indeterminant
// Observation2: For many non column wins of the same player earliest can be determined as COLS / 2 < WINS

int drill(const char p, const int r, const int c, const int dr, const int dc)
{
    if (r == -1 || r == ROWS || c == -1 || c == COLS || board[r][c] != p)
        return 0;

    return 1 + drill(p, r + dr, c + dc, dr, dc);
}

void drill_check(const char p, const int r, const int c)
{
    for (int dir = 0, dr2 = 4; dir < 4; ++dir, ++dr2) {
        const int a = drill(p, r + DR[dir], c + DC[dir], DR[dir], DC[dir]);
        const int b = drill(p, r + DR[dr2], c + DC[dr2], DR[dr2], DC[dr2]);
        if (WINS <= a + 1 + b) {
            if (p == 'C')
                cwon = true;
            else
                fwon = true;
        }
    }
}

bool wins(const char p)
{
    for (int c = 0; c < COLS; ++c) {
        int consecutive{};
        for (int r = 0; r < ROWS; ++r)
            if (board[r][c] == p) {
                if (++consecutive == WINS)
                    return true;
            } else
                consecutive = 0;
    }

    for (int r = 0; r < ROWS; ++r) {
        int consecutive{};
        for (int c = 0; c < COLS; ++c)
            if (board[r][c] == p) {
                if (++consecutive == WINS)
                    return true;
            } else
                consecutive = 0;
    }

    for (int d = WINS - 1; d < ROWS + COLS - 1 - WINS - 1; ++d) {
        int r = d < ROWS ? d : ROWS - 1;
        int c = d < ROWS ? 0 : d - ROWS + 1;
        int consecutive{};
        while (0 <= r && c < COLS) {
            if (board[r][c] == p) {
                if (++consecutive == WINS)
                    return true;
            } else
                consecutive = 0;
            --r;
            ++c;
        }
    }

    for (int d = WINS - 1; d < ROWS + COLS - 1 - WINS - 1; ++d) {
        int r = d < ROWS ? d : ROWS - 1;
        int c = d < ROWS ? COLS - 1 : COLS - (d - ROWS + 1) - 1;
        int consecutive{};
        while (0 <= r && 0 <= c) {
            if (board[r][c] == p) {
                if (++consecutive == WINS)
                    return true;
            } else
                consecutive = 0;
            --r;
            --c;
        }
    }

    return false;
}

char answer()
{
    if (!cwon && !fwon)
        return '0';
    if (cwon && !fwon)
        return 'C';
    if (fwon && !cwon)
        return 'F';
    return '?';
}

void dfs(const char p, const int ord)
{
    if (ord == SIZE)
        return;
    // if in cache return

    for (int c = 0; c < COLS; ++c) {
        auto& r = column_height[c];
        if (board[r][c] == p) {
            ++r;
            dfs(p == 'C' ? 'F' : 'C', ord + 1);
            --r;
        }
    }
}

static char solve()
{
    for (auto& p : board)
        std::cin >> p;

    std::reverse(board.begin(), board.end());

    // Check simple cases
    cwon = wins('C');
    fwon = wins('F');
    char ans = answer();
    if (ans != '?')
        return ans;

    // Run simulation
    cwon = fwon = false;
    column_height.fill(0);
    dfs('C', 0);

    return answer();
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g)
        std::cout << std::format("Case #{}: {}\n", g, solve());
}

/*

Format:
clang-format -i cottontail_climb_2.cpp

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 four_in_burrow.cpp
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address four_in_burrow.cpp

Run:
a.exe < four_in_burrow.in

Input:

4

FFFFFFF
CCCCCCC
FFFFFFF
CCCCCCC
FFFFFFF
CCCCCCC

FCFCFCF
FCCFCFC
CFFCFCF
CFCFCFC
CFCFFCF
CFCFCFC

FCFCFCF
CCFCFCF
CFCFCCF
CFFFCFC
FCCCCCC
CFFFFFF

FCFCFCF
CFCFCFC
FCFCFCF
FCFCFCF
CFCFCFC
CFCFCFC

Output:

Case #1: C
Case #2: ?
Case #3: F
Case #4: 0

*/

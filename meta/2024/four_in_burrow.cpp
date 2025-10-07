
#include <algorithm>
#include <array>
#include <iostream>
#include <format>
#include <unordered_set>

// Four in a Burrow
// https://www.facebook.com/codingcompetitions/hacker-cup/2024/round-2/problems/B

// A win in a game that cannot be continued shall be ignored

static constexpr int WINS = 4;
static constexpr int ROWS = 6;
static constexpr int COLS = 7;
static constexpr int SIZE = ROWS * COLS;
static constexpr const char* ANS = "0CF?";
static constexpr int DR[] = {1, 1, 1, 0, -1, -1, -1, 0};
static constexpr int DC[] = {1, 0, -1, -1, -1, 0, 1, 1};

int ans; // itmask: C==1, F==2
std::unordered_set<int> cache;
std::array<int, COLS> column_height;
std::array<std::string, ROWS> board;

int play_hash(const int winner)
{
    int ret = winner << (COLS * 4);
    for (int c = 0; c < COLS; ++c)
        ret |= column_height[c] << (c * 4);
    return ret;
}

int drill(const char p, const int r, const int c, const int dr, const int dc)
{
    if (c == -1 || c == COLS || r == -1 || r >= column_height[c] || board[r][c] != p)
        return 0;

    return 1 + drill(p, r + dr, c + dc, dr, dc);
}

int drill_check(const char p, const int r, const int c)
{
    for (int dir = 0, dr2 = 4; dir < 4; ++dir, ++dr2) {
        const int a = drill(p, r + DR[dir], c + DC[dir], DR[dir], DC[dir]);
        const int b = drill(p, r + DR[dr2], c + DC[dr2], DR[dr2], DC[dr2]);
        if (WINS <= a + 1 + b)
            return (p == 'C') ? 1 : 2;
    }

    return 0;
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

    constexpr const auto diagonals_end = ROWS + COLS - 1 - WINS + 1;
    for (int d = WINS - 1; d < diagonals_end; ++d) {
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

    for (int d = WINS - 1; d < diagonals_end; ++d) {
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

void dfs(const int ord, int winner = 0)
{
    if (ord == SIZE) {
        ans |= winner;
        return;
    }

    const auto [_, inserted] = cache.insert(play_hash(winner));
    if (!inserted)
        return;

    const char p = (ord & 1) ? 'F' : 'C';
    for (int c = 0; c < COLS; ++c) {
        auto& r = column_height[c];
        if (r < ROWS && board[r][c] == p) {
            auto local_winner = winner;
            if (!winner)
                local_winner = drill_check(p, r, c);
            ++r;
            dfs(ord + 1, local_winner);
            --r;
        }
    }
}

static char solve()
{
    for (auto& p : board)
        std::cin >> p;

    std::reverse(board.begin(), board.end());

    // Check simple cases first
    ans = 0;
    if (wins('C'))
        ans |= 1;
    if (wins('F'))
        ans |= 2;
    if (ans == 3) { // ? requires running a simulation
        ans = 0;
        cache.clear();
        column_height.fill(0);
        dfs(0);
    }

    return ANS[ans];
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
clang-format -i four_in_burrow.cpp

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 four_in_burrow.cpp
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address four_in_burrow.cpp

Run:
a.exe < four_in_burrow.in

Input:

1
CFFFFFF
CFFCFFF
CCCFFCC
CCFFCCC
FFCFCCF
CCCFCFC

Output:

Case #1: F

*/

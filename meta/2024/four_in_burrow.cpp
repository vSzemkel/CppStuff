
#include <array>
#include <iostream>
#include <format>
#include <vector>

// Cottontail Climb - part II
// https://www.facebook.com/codingcompetitions/hacker-cup/2024/round-2/problems/B


static constexpr int WINS = 4;
static constexpr int ROWS = 6;
static constexpr int COLS = 7;
static constexpr int SIZE = ROWS * COLS;
using board_t = std::array<char, SIZE>;

board_t board;

bool wins(const char p)
{
    for (int r = 0; r < ROWS; ++r) {
        int consecutive{};
        for (int c = 0; c < COLS; ++c) {
            if (board[r * COLS + c] == p)
                ++consecutive;
            else
                consecutive = 0;
            if (consecutive == WINS)
                return true;
        }
    }

    for (int c = 0; c < COLS; ++c) {
        int consecutive{};
        for (int r = 0; r < ROWS; ++r) {
            if (board[r * COLS + c] == p)
                ++consecutive;
            else
                consecutive = 0;
            if (consecutive == WINS)
                return true;
        }
    }

    for (int d = WINS - 1; d < ROWS + COLS - 1 - WINS - 1; ++d) {
        int rd = d < ROWS ? d : ROWS - 1;
        int cd = d < ROWS ? 0 : d - ROWS + 1;
        int consecutive{};
        while (0 <= rd && cd < COLS) {
            if (board[rd * COLS + cd] == p)
                ++consecutive;
            else
                consecutive = 0;
            if (consecutive == WINS)
                return true;
            --rd;
            ++cd;
        }
    }

    for (int d = WINS - 1; d < ROWS + COLS - 1 - WINS - 1; ++d) {
        int rd = d < ROWS ? d : ROWS - 1;
        int cd = d < ROWS ? COLS - 1 : COLS - (d - ROWS + 1) - 1;
        int consecutive{};
        while (0 <= rd && cd < COLS) {
            if (board[rd * COLS + cd] == p)
                ++consecutive;
            else
                consecutive = 0;
            if (consecutive == WINS)
                return true;
            --rd;
            --cd;
        }
    }

    return false;
}

static char solve()
{

    for (auto& p : board)
        std::cin >> p;

    const bool cwon = wins('C');
    const bool fwon = wins('F');

    if (!cwon && !fwon)
        return '0';
    if (cwon && !fwon)
        return 'C';
    if (!cwon && fwon)
        return 'F';

    // TODO: run simulation

    return '?';
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

*/

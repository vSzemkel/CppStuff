
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
using board_t = std::array<char, SIZE>;

board_t board;
std::unordered_map<char, std::array<int, COLS>> best; // best['F'][3] is the earlist row number F won having burrow in 3rd column

// Observation1: Order of wins by column is indeterminant
// Observation2: For many non column wins of the same player earliest can be determined as COLS / 2 < WINS

bool wins(const char p)
{
    bool ret{};
    auto& b = best[p];

    // Ignore column wins order 
    for (int c = 0; c < COLS; ++c) {
        int consecutive{};
        for (int r = 0; r < ROWS; ++r) {
            if (board[r * COLS + c] == p) {
                if (++consecutive == WINS)
                    ret = true;
            } else
                consecutive = 0;
        }
    }

    for (int r = 0; r < ROWS; ++r) {
        int consecutive{};
        for (int c = 0; c < COLS; ++c) {
            if (board[r * COLS + c] == p) {
                if (++consecutive == WINS) {
                    if (b[c - 3] < r || b[c - 2] < r || b[c - 1] < r || b[c] < r)
                        b[c - 3] = b[c - 2] = b[c - 1] = b[c] = r;
                    --consecutive;
                    ret = true;
                }
            } else
                consecutive = 0;
        }
    }

    for (int d = WINS - 1; d < ROWS + COLS - 1 - WINS - 1; ++d) {
        int r = d < ROWS ? d : ROWS - 1;
        int c = d < ROWS ? 0 : d - ROWS + 1;
        int consecutive{};
        while (0 <= r && c < COLS) {
            if (board[r * COLS + c] == p) {
                if (++consecutive == WINS) {
                    if (b[c - 3] < r + 3 || b[c - 2] < r + 2 || b[c - 1] < r + 1 || b[c] < r) {
                        b[c - 3] = r + 3;
                        b[c - 2] = r + 2;
                        b[c - 1] = r + 1;
                        b[c] = r;
                    }
                    --consecutive;
                    ret = true;
                }
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
            if (board[r * COLS + c] == p) {
                if (++consecutive == WINS) {
                    if (b[c + 3] < r + 3 || b[c + 2] < r + 2 || b[c + 1] < r + 1 || b[c] < r) {
                        b[c + 3] = r + 3;
                        b[c + 2] = r + 2;
                        b[c + 1] = r + 1;
                        b[c] = r;
                    }
                    --consecutive;
                    ret = true;
                }
            } else
                consecutive = 0;
            --r;
            --c;
        }
    }

    return ret;
}

static char solve()
{

    for (auto& p : board)
        std::cin >> p;

    best['C'].fill(-1);
    best['F'].fill(-1);
    const bool cwon = wins('C');
    const bool fwon = wins('F');

    if (!cwon && !fwon)
        return '0';
    if (cwon && !fwon)
        return 'C';
    if (!cwon && fwon)
        return 'F';

    int consecutiveC{};
    int consecutiveF{};
    for (int c = 0; c < COLS; ++c) {
        if (best['C'] > best['F']) {
            ++consecutiveC;
            consecutiveF = 0;
        } else {
            ++consecutiveF;
            consecutiveC = 0;
        }
        if (consecutiveC == WINS)
            return 'C';
        if (consecutiveF == WINS)
            return 'F';
    }

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

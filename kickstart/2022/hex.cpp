
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// Hex
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008f4332/0000000000942527

// Blue wins horizontally
bool bwins, rwins;
int N, size, bcnt, rcnt; // stone counts
std::vector<std::string> board;

const int dx[6] = {1, 1, 0, -1, -1, 0};
const int dy[6] = {0, -1, -1, 0, 1, 1};

void flood_fill(std::vector<std::string>& board, const int r, const int c) {
    auto& cell = board[r][c];
    const char color = cell;
    cell ^= 0x20; // to_lower
    for (int i = 0; i < 6; ++i) {
        const int nr = r + dy[i];
        const int nc = c + dx[i];
        if (0 <= nr && nr < N && 0 <= nc && nc < N && board[nr][nc] == color)
            flood_fill(board, nr, nc);
    }
}

static char winner(const std::vector<std::string>& board) {
    if (N == 1)
        return board[0][0];

    auto copy = board;
    for (int i = 0; i < N; ++i) {
        if (copy[i][0] == 'B')
            flood_fill(copy, i, 0);
        if (copy[0][i] == 'R')
            flood_fill(copy, 0, i);
    }

    for (int i = 0; i < N; ++i) {
        if (copy[i][N - 1] == 'b')
            return 'B';
        if (copy[N - 1][i] == 'r')
            return 'R';
    }

    return '.';
}

static std::string solve() {
    std::cin >> N;
    size = N * N;
    board.resize(N);
    for (auto& r : board)
        std::cin >> r;

    bcnt = rcnt = 0;
    for (const auto& r : board) {
        bcnt += std::count(r.begin(), r.end(), 'B');
        rcnt += std::count(r.begin(), r.end(), 'R');
    }
    if (std::abs(bcnt - rcnt) > 1)
        return "Impossible";

    const char won = winner(board);
    if (won == 'B' && bcnt < rcnt)
        return "Impossible";
    if (won == 'R' && rcnt < bcnt)
        return "Impossible";
    if (won == '.')
        return "Nobody wins";

    bool dub{true}; // after removing winning move, boards must be not solved
    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c) {
            auto& cell = board[r][c];
            if (cell == won) {
                cell = '.';
                if (winner(board) != won)
                    dub = false;
                cell = won;
            }
        }

    if (dub)
        return "Impossible";
    if (won == 'B')
        return "Blue wins";

    return "Red wins";
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": " << solve() << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 hex.cpp -o hex.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address hex.cpp -o hex

Run:
hex.exe < hex.in

Input:

7
1
.
1
B
1
R
2
BR
BB
4
BBBB
BBB.
RRR.
RRRR
4
BBBB
BBBB
RRR.
RRRR
6
......
..R...
BBBBBB
..R.R.
..RR..
......

Output:


*/

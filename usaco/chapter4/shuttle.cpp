/*
ID: marcin.8
LANG: C++
TASK: shuttle
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=MlF4Dw33eOh&S=shuttle
*/

#include <algorithm>
#include <assert.h>
#include <fstream>
#include <string>
#include <vector>

std::ifstream task_in("shuttle.in");
std::ofstream task_out("shuttle.out");

constexpr const int LINESIZE = 20;
std::string board;
int N;

static bool expected() {
    return std::all_of(board.begin(), board.begin() + N, [](const char c){ return c == 'B'; })
        && board[N] == ' ';
}

static bool valid() {
    // optimizations not needed for this task constraints
    return true;
}

/**
 * @brief 2 strokes are better then 1 strokes
 * Leftmost move with equal stroke wins
 */
int main(int, char**)
{
    task_in >> N;
    const int size = 2 * N + 1;
    board.assign(size, 'W');
    int hole = N;
    board[hole] = ' ';
    for (int i = N + 1; i < size; ++i)
        board[i] = 'B';

    const int max_moves = N * (N + 1);
    std::vector<char> rolled_back(max_moves), types;
    std::vector<int> moves;
    moves.reserve(max_moves);
    types.reserve(max_moves);
    while (true) {
        // std::cout << board << std::endl;
        const auto cur = moves.size();
        if (rolled_back.size() == cur)
            rolled_back.push_back({});
        if ((rolled_back[cur] & 1) == 0 && hole > 1 && board[hole - 2] == 'W' && board[hole - 1] == 'B') {
            std::swap(board[hole - 2], board[hole]);
            hole -= 2;
            if (valid()) {
                types.push_back(1);
                moves.push_back(hole);
                continue;
            } else {
                hole += 2;
                std::swap(board[hole - 2], board[hole]);
            }
        }

        if ((rolled_back[cur] & 2) == 0 && hole < size - 2 && board[hole + 1] == 'W' && board[hole + 2] == 'B') {
            std::swap(board[hole], board[hole + 2]);
            hole += 2;
            if (valid()) {
                types.push_back(2);
                moves.push_back(hole);
                continue;
            } else {
                hole -= 2;
                std::swap(board[hole], board[hole + 2]);
            }
        }

        if ((rolled_back[cur] & 4) == 0 && hole > 0 && board[hole - 1] == 'W') {
            std::swap(board[hole - 1], board[hole]);
            --hole;
            if (valid()) {
                types.push_back(4);
                moves.push_back(hole);
                continue;
            } else {
                ++hole;
                std::swap(board[hole - 1], board[hole]);
            }
        }

        if ((rolled_back[cur] & 8) == 0 && hole < size - 1 && board[hole + 1] == 'B') {
            std::swap(board[hole], board[hole + 1]);
            ++hole;
            if (valid()) {
                types.push_back(8);
                moves.push_back(hole);
                continue;
            } else {
                --hole;
                std::swap(board[hole], board[hole + 1]);
            }
        }

        if (expected())
            break;

        assert(cur > 0);
        const auto rb = types.back();
        rolled_back[cur - 1] |= rb;
        rolled_back[cur] = 0;
        types.pop_back();
        moves.pop_back();

        switch (rb) {
            case 1: std::swap(board[hole], board[hole + 2]); hole += 2; break;
            case 2: std::swap(board[hole - 2], board[hole]); hole -= 2; break;
            case 4: std::swap(board[hole], board[hole + 1]); ++hole; break;
            case 8: std::swap(board[hole - 1], board[hole]); --hole; break;
        }
        // std::cout << "ROLLBACK " << cur << ' ';
    }

    assert(expected());
    int pos{0};
    for (const int m : moves)
        task_out << m + 1 << (++pos % LINESIZE ? ' ' : '\n');
    task_out.seekp(-1, std::ios_base::cur);
    task_out << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 shuttle.cpp -o shuttle.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address shuttle.cpp -o shuttle

Input:


Output:


*/

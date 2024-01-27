/*
ID: marcin.8
LANG: C++
TASK: snail
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=3UtkCxcOie8&S=snail
*/

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdlib>
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

std::ifstream task_in("snail.in");
std::ofstream task_out("snail.out");

static const char FREE = '.';
static const char BARIER = '#';
static const char VISITED = 'o';
static const std::pair<int, int> DELTA[] = {
    {0, -1}, {1, 0}, {0, 1}, {-1, 0} // NESW, d + 2 is opposite to d
};
int S, ans{-1}, score{1};
std::vector<std::string> board;

static void advance(int row, int col, int dir)
{
    bool progress{};
    const auto [dx, dy] = DELTA[dir];
    int r = row + dy, c = col + dx;
    while (0 <= r && r < S && 0 <= c && c < S && board[r][c] == FREE) {
        board[r][c] = VISITED;
        ++score;
        progress = true;
        r += dy, c += dx;
    }

    if (progress) {
        r -= dy, c -= dx;
        advance(r, c, (dir + 1) % 4);
        advance(r, c, (dir + 3) % 4);

        // backtrack
        r = row + dy;
        c = col + dx;
        while (0 <= r && r < S && 0 <= c && c < S && board[r][c] == VISITED) {
            board[r][c] = FREE;
            --score;
            r += dy, c += dx;
        }
    } else
        ans = std::max(ans, score);
}

int main(int, char**)
{
    int B;
    task_in >> S >> B >> std::ws;
    board.assign(S, std::string(S, FREE));
    for (; B; --B) {
        int r;
        char c;
        task_in >> c >> r;
        board[--r][c - 'A'] = BARIER; 
    }

    advance(0, 0, 1);
    advance(0, 0, 2);
    task_out << ans << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 snail.cpp -o snail.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address snail.cpp -o snail

Input:


Output:


*/

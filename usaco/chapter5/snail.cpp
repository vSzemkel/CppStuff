/*
ID: marcin.8
LANG: C++
TASK: snail
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=3UtkCxcOie8&S=snail
*/

#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

std::ifstream task_in("snail.in");
std::ofstream task_out("snail.out");

static const char FREE = '.';
static const char BARIER = '#';
static const char VISITED = 'o';
static const int DELTAX[] = {0, 1, 0, -1}; // NESW, d + 2 is opposite to d
static const int DELTAY[] = {-1, 0, 1, 0};

int S, ans{0}, score{1};
std::vector<std::string> board;

/*
 * This task has second version in which hit is always true.
 * Version below is harder because of two kinds of possible stops
*/
static void advance(const int row, const int col, const int dir)
{
    const int init_score = score;
    const int dx = DELTAX[dir], dy = DELTAY[dir];
    int r = row + dy, c = col + dx;
    while (0 <= r && r < S && 0 <= c && c < S && board[r][c] == FREE) {
        board[r][c] = VISITED;
        ++score;
        r += dy, c += dx;
    }

    if (init_score < score) {
        const bool hit = r < 0 || S <= r || c < 0 || S <= c || board[r][c] == BARIER;
        if (hit) {
            r -= dy, c -= dx;
            advance(r, c, (dir + 1) % 4);
            advance(r, c, (dir + 3) % 4);
        } else 
            ans = std::max(ans, score); // not hit exit

        // backtrack
        r = row + dy;
        c = col + dx;
        while (init_score < score) {
            board[r][c] = FREE;
            --score;
            r += dy, c += dx;
        }
    } else
        ans = std::max(ans, score); // hit exit
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

    if (board[0][0] == FREE) {
        board[0][0] = VISITED;
        advance(0, 0, 1);
        advance(0, 0, 2);
    }

    task_out << ans << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 snail.cpp -o snail.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address snail.cpp -o snail

Input:

8 4
E2
A6
G1
F5

Output:

33

*/

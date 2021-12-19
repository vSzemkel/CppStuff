/*
ID: marcin.8
LANG: C++
TASK: maze1
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=M9tbSYKaKlb&S=maze1
*/

#include <array>
#include <assert.h>
#include <fstream>
#include <queue>
#include <string>
#include <vector>

std::ifstream task_in("maze1.in");
std::ofstream task_out("maze1.out");

std::vector<std::vector<int>> ans;
std::vector<std::vector<bool>> down, right;

int main(int, char**)
{
    int W, H;
    std::string line_h, line_v;
    task_in >> W >> H;
    const int MAX = 2 * W * H;
    std::getline(task_in, line_h); // new line character
    down.assign(H + 1, std::vector<bool>(W + 1, true));
    right = down;
    ans.assign(H + 1, std::vector<int>(W + 1, MAX));

    // read the maze
    std::vector<std::pair<int, int>> entries;
    std::getline(task_in, line_h);
    for (int c = 1; c <= W; ++c)
        if (line_h[2 * c - 1] != '-')
            entries.emplace_back(1, c);
    for (int r = 1; r <= H; ++r) {
        std::getline(task_in, line_v);
        std::getline(task_in, line_h);
        for (int c = 1; c <= W; ++c) {
            if (line_v[2 * c] != '|')
                right[r][c] = false;
            if (line_h[2 * c - 1] != '-')
                down[r][c] = false;
        }
        if (line_v[0] != '|')
            entries.emplace_back(r, 1);
        if (right[r][W] == false)
            entries.emplace_back(r, W);
    }
    for (int c = 1; c <= W; ++c)
        if (line_h[2 * c - 1] != '-')
            entries.emplace_back(H, c);

    // BFS for each entrance
    assert(entries.size() == 2);
    for (const auto& start : entries) {
        std::queue<std::array<int, 3>> qq; // {r, c, dist}
        std::vector<std::vector<bool>> seen(H + 1, std::vector<bool>(W + 1));
        seen[start.first][start.second] = true;
        qq.push({start.first, start.second, 1});
        while (!qq.empty()) {
            const auto& cell = qq.front();
            ans[cell[0]][cell[1]] = std::min(ans[cell[0]][cell[1]], cell[2]);
            if (cell[1] > 1 && !right[cell[0]][cell[1] - 1] && !seen[cell[0]][cell[1] - 1]) {
                seen[cell[0]][cell[1] - 1] = true;
                qq.push({cell[0], cell[1] - 1, cell[2] + 1});
            }
            if (cell[1] < W && !right[cell[0]][cell[1]] && !seen[cell[0]][cell[1] + 1]) {
                seen[cell[0]][cell[1] + 1] = true;
                qq.push({cell[0], cell[1] + 1, cell[2] + 1});
            }
            if (cell[0] > 1 && !down[cell[0] - 1][cell[1]] && !seen[cell[0] - 1][cell[1]]) {
                seen[cell[0] - 1][cell[1]] = true;
                qq.push({cell[0] - 1, cell[1], cell[2] + 1});
            }
            if (cell[0] < H && !down[cell[0]][cell[1]] && !seen[cell[0] + 1][cell[1]]) {
                seen[cell[0] + 1][cell[1]] = true;
                qq.push({cell[0] + 1, cell[1], cell[2] + 1});
            }
            qq.pop();
        }
    }

    int ret{0};
    for (int r = 1; r <= H; ++r)
        for (int c = 1; c <= W; ++c)
            if (ans[r][c] < MAX)
                ret = std::max(ret, ans[r][c]);

    task_out << ret << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 maze1.cpp -o maze1.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address maze1.cpp -o maze1

Input:

5 3
+-+-+-+-+-+
|         |
+-+ +-+ + +
|     | | |
+ +-+-+ + +
| |     |  
+-+ +-+-+-+

Output:

9

*/

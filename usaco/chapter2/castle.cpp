/*
ID: marcin.8
LANG: C++
TASK: castle
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=TAd8FDVE2eC&S=castle
*/

#include <algorithm>
#include <array>
#include <assert.h>
#include <bitset>
#include <cmath>
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
#include <stdlib.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <vector>

std::ifstream task_in("castle.in");
std::ofstream task_out("castle.out");

int C, R;
constexpr auto fn_col = [](const int cell){ return cell % C; };
constexpr auto fn_row = [](const int cell){ return cell / C; };

std::vector<int> adjacents(const int pos, const int border)
{
    const int row = fn_row(pos);
    const int col = fn_col(pos);

    std::vector<int> adj;
    if (col > 0)
        adj.push_back(border & 1 ? 1 - pos : pos - 1); // W
    if (col < C - 1)
        adj.push_back(border & 4 ? -1 - pos : pos + 1); // E
    if (row > 0)
        adj.push_back(border & 2 ? C - pos : pos - C); // N
    if (row < R - 1)
        adj.push_back(border & 8 ? -C - pos: pos + C); // S

    return adj;
}


int main(int, char**)
{
    task_in >> C >> R;
    const int size = R * C;
    std::vector<bool> visited(size);
    std::set<std::pair<int, int>> adjacent; // {region1, region2}
    std::vector<int> dfs, sizes, board(size), region(size, -1);
    for (auto& c : board)
        task_in >> c;

    int rgn{0}, maxroom{0};
    for (int i = 0; i < size; ++i)
        if (!visited[i]) {
            int cnt{1};
            region[i] = rgn;
            visited[i] = true;
            dfs.assign(1, i);
            while (!dfs.empty()) {
                const int cur = dfs.back();
                dfs.pop_back();
                for (const int next : adjacents(cur, board[cur])) {
                    if (next < 0) {
                        int neigh = -next;
                        if (visited[neigh] && region[neigh] != rgn)
                            adjacent.emplace(region[neigh], rgn);
                        continue;
                    }
                    if (!visited[next]) {
                        region[next] = rgn;
                        visited[next] = true;
                        dfs.push_back(next);
                        ++cnt;
                    }
                }
            }
            sizes.push_back(cnt);
            maxroom = std::max(maxroom, cnt);
            ++rgn;
        }

    int regA, regB, unionsize{-1};
    for (const auto& a : adjacent) {
        const int total = sizes[a.first] + sizes[a.second];
        if (unionsize < total) {
            unionsize = total;
            regA = a.first;
            regB = a.second;
        }
    }

    task_out << rgn << '\n';
    task_out << maxroom << '\n';
    task_out << unionsize << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 castle.cpp -o castle.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address castle.cpp -o castle

Input:

7 4
11 6 11 6 3 10 6
7 9 6 13 5 15 5
1 10 12 7 13 7 5
13 11 10 8 10 12 13

Output:

5
9
16
4 1 E

*/

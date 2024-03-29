/*
ID: marcin.8
LANG: C++
TASK: castle
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=TAd8FDVE2eC&S=castle
*/

#include <algorithm>
#include <fstream>
#include <map>
#include <utility>
#include <vector>

std::ifstream task_in("castle.in");
std::ofstream task_out("castle.out");

int C, R;
auto fn_col = [](const int cell){ return cell % C; };
auto fn_row = [](const int cell){ return cell / C; };
auto order = [](const int a1, const int a2) { // prefer W, E
    const int r1 = fn_row(a1);
    const int c1 = fn_col(a1);
    const int r2 = fn_row(a2);
    const int c2 = fn_col(a2);
    return c1 < c2 || (c1 == c2 && r1 > r2);
};

std::vector<int> adjacents_greedy(const int pos, const int border) // {neigbor, is_behind_wall}
{
    const int row = fn_row(pos);
    const int col = fn_col(pos);

    std::vector<int> adj;
    if (col > 0 && (border & 1) == 0)
        adj.push_back(pos - 1); // W
    if (col < C - 1 && (border & 4) == 0)
        adj.push_back(pos + 1); // E
    if (row > 0 && (border & 2) == 0)
        adj.push_back(pos - C); // N
    if (row < R - 1 && (border & 8) == 0)
        adj.push_back(pos + C); // S

    return adj;
}

void greedy(int, char**)
{
    task_in >> C >> R;
    const int size = R * C;
    std::vector<bool> visited(size);
    std::vector<int> dfs, sizes, board(size), region(size, -1);
    std::map<std::pair<int, int>, std::vector<int>> adjacent; // {region1, region2} => [r1 cells]
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
                for (const auto next : adjacents_greedy(cur, board[cur]))
                    if (!visited[next]) {
                        region[next] = rgn;
                        visited[next] = true;
                        dfs.push_back(next);
                        ++cnt;
                    }
            }

            maxroom = std::max(maxroom, cnt);
            sizes.push_back(cnt);
            ++rgn;
        }

    char wall;
    int ret_r, ret_c, unionsize{-1};
    for (int c = 0; c < C; ++c)
        for (int r = R - 1; r >= 0; --r) {
            const int win_cell = r * C + c;
            const int win_reg = region[win_cell];
            const int win_size = sizes[win_reg]; 
            if (r > 0) {
                const int can_reg = region[win_cell - C];
                const int can_size = sizes[can_reg] + win_size;
                if (can_reg != win_reg && can_size > unionsize) {
                    ret_r = r;
                    ret_c = c;
                    wall = 'N';
                    unionsize = can_size;
                }
            }
            if (c < C - 1) {
                const int can_reg = region[win_cell + 1];
                const int can_size = sizes[can_reg] + win_size;
                if (can_reg != win_reg && can_size > unionsize) {
                    ret_r = r;
                    ret_c = c;
                    wall = 'E';
                    unionsize = can_size;
                }
            }
        }

    task_out << rgn << '\n';
    task_out << maxroom << '\n';
    task_out << unionsize << '\n';
    task_out << ret_r + 1 << ' ' << ret_c + 1 << ' ' << wall << '\n';
}

std::vector<std::pair<int, bool>> adjacents(const int pos, const int border) // {neigbor, is_begind_wall}
{
    const int row = fn_row(pos);
    const int col = fn_col(pos);

    std::vector<std::pair<int, bool>> adj;
    if (col > 0)
        adj.emplace_back(pos - 1, border & 1); // W
    if (col < C - 1)
        adj.emplace_back(pos + 1, border & 4); // E
    if (row > 0)
        adj.emplace_back(pos - C, border & 2); // N
    if (row < R - 1)
        adj.emplace_back(pos + C, border & 8); // S

    return adj;
}

int main(int, char**)
{
    task_in >> C >> R;
    const int size = R * C;
    std::vector<bool> visited(size);
    std::vector<int> dfs, sizes, board(size), region(size, -1);
    std::map<std::pair<int, int>, std::vector<int>> adjacent; // {region1, region2} => [r1 cells on border with r2]
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
                for (const auto& [next, wall] : adjacents(cur, board[cur])) {
                    if (wall) {
                        if (visited[next] && region[next] != rgn)
                            adjacent[{region[next], rgn}].push_back(next);
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

            maxroom = std::max(maxroom, cnt);
            sizes.push_back(cnt);
            ++rgn;
        }

    int regB{-1}, cellA{-1}, unionsize{-1};
    for (const auto& [a, b] : adjacent) {
        const int total = sizes[a.first] + sizes[a.second];
        if (total < unionsize)
            continue;
        const int cell = *std::min_element(b.begin(), b.end(), order);
        if (total == unionsize && order(cellA, cell))
            continue;
        cellA = cell;
        regB = a.second;
        unionsize = total;
    }

    task_out << rgn << '\n';
    task_out << maxroom << '\n';
    task_out << unionsize << '\n';

    char wall{'N'};
    int r = fn_row(cellA);
    int c = fn_col(cellA);
    while (true) {
        if (r > 0 && region[cellA - C] == regB)
            break;
        if (r < R - 1 && region[cellA + C] == regB) {
            ++r;
            break;
        }
        wall = 'E';
        if (c < C - 1 && region[cellA + 1] == regB)
            break;
        if (c > 0 && region[cellA - 1] == regB) {
            --c;
            break;
        }
    }

    if (wall == 'E' && r > 0) {
        const int win_cell = r * C + c;
        const int win_reg = region[win_cell];
        const int can_reg = region[win_cell - C];
        if (can_reg != win_reg && sizes[can_reg] + sizes[win_reg] == unionsize)
            wall = 'N';
    }

    task_out << r + 1 << ' ' << c + 1 << ' ' << wall << '\n';
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

/*
ID: marcin.8
LANG: C++
TASK: camelot
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=unnUOe16hgm&S=camelot
*/

#include <algorithm>
#include <array>
#include <assert.h>
#include <fstream>
#include <numeric>
#include <queue>
#include <vector>

std::ifstream task_in("camelot.in");
std::ofstream task_out("camelot.out");

constexpr int INF = 1e09;

std::vector<int> knight_distance;
std::vector<bool> in_queue;
std::queue<int> qq;

using pos_t = std::pair<int, int>;

const pos_t singularity{1, 1};

static pos_t diff(const pos_t& pos, const int r, const int c) {
    return {std::abs(pos.first - r), std::abs(pos.second - c)};
}

/**
 * @brief Precompute knight distances between every two cells on board
 * 
 * @param rows Board's height
 * @param cols Board's width
 */
static void init_knight_distance(const int rows, const int cols) {
    const auto sz = rows * cols;
    knight_distance.assign(sz, INF);
    in_queue.assign(sz, false);

    const auto dijkstra = [](const int cost, const int next) {
        if (cost < knight_distance[next]) {
            knight_distance[next] = cost;
            if (!in_queue[next]) {
                in_queue[next] = true;
                qq.push(next);
            }
        }
    };

    knight_distance[0] = 0;
    qq.push(0);
    in_queue[0] = true;
    while (!qq.empty()) {
        const int cell = qq.front(); qq.pop();
        in_queue[0] = false;
        const int r = cell / cols;
        const int c = cell % cols;
        const int cost = knight_distance[r * cols + c] + 1;
        if (c + 2 < cols) {
            if (r > 0) dijkstra(cost, (r - 1) * cols + c + 2);
            if (r < rows - 1) dijkstra(cost, (r + 1) * cols + c + 2);
        }
        if (0 <= c - 2) {
            if (r > 0) dijkstra(cost, (r - 1) * cols + c - 2);
            if (r < rows - 1) dijkstra(cost, (r + 1) * cols + c - 2);
        }
        if (r + 2 < rows) {
            if (c > 0) dijkstra(cost, (r + 2) * cols + c - 1);
            if (c < cols - 1) dijkstra(cost, (r + 2) * cols + c + 1);
        }
        if (0 <= r - 2) {
            if (c > 0) dijkstra(cost, (r - 2) * cols + c - 1);
            if (c < cols - 1) dijkstra(cost, (r - 2) * cols + c + 1);
        }
    }
}

int main(int, char**)
{
    int R, C;
    task_in >> R >> C;
    int r; char c;
    task_in >> c >> r >> std::ws;
    pos_t king = {r - 1, c - 'A'};
    std::vector<pos_t> knights;
    while (task_in.good()) {
        task_in >> c >> r >> std::ws;
        knights.emplace_back(r - 1, c - 'A');
    }

    if (knights.empty()) {
        task_out << "0\n";
        return 0;
    }

    init_knight_distance(R, C);

    int ans = INF;
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c) {
            int can{0};
            for (const auto& k : knights) {
                const auto d = diff(k, r, c);
                if (d == singularity && !(r == 0 || c == 0 || r == R - 1 || c == C - 1 || k.first == 0 || k.second == 0 || k.first == R - 1 || k.second == C - 1))
                    can += 2;
                else
                    can += knight_distance[d.first * C + d.second];
            }
            ans = std::min(ans, can);
        }

    task_out << ans << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 camelot.cpp -o camelot.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address camelot.cpp -o camelot

Input:


Output:


*/

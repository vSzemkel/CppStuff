
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// L Shaped Plots
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000436140/000000000068c509

int64_t g_rows, g_cols;
std::vector<int> g_board;
const auto col = [](const int64_t pos) noexcept { return pos % g_cols; };
const auto row = [](const int64_t pos) noexcept { return pos / g_cols; };

static int64_t advance(const int64_t pos, const char dir)
{
    switch (dir) {
        case 'E': return (col(pos) + 1 == g_cols) ? -1 : pos + 1;
        case 'W': return (col(pos) == 0) ? -1 : pos - 1;
        case 'S': return (row(pos) + 1 == g_rows) ? -1 : pos + g_cols;
        case 'N': return (row(pos) == 0) ? -1 : pos - g_cols;
    }
    return -1;
}

static void solve() {
    std::cin >> g_rows >> g_cols;
    int size = g_rows * g_cols;
    std::unordered_map<char, std::vector<int>> neighbours;
    g_board.resize(size);
    neighbours['N'].resize(size);
    neighbours['E'].resize(size);
    neighbours['S'].resize(size);
    neighbours['W'].resize(size);
    for (int i = 0; i < size; ++i) {
        int taken; std::cin >> taken;
        g_board[i] = taken;
        if (taken)
            neighbours['N'][i] = neighbours['E'][i] = neighbours['S'][i] = neighbours['W'][i] = 1;
    }

    for (int64_t r = 0; r < g_rows; ++r)
        for (int64_t c = 0; c < g_cols; ++c) {
            const int64_t cell = r * g_cols + c;
            if (!g_board[cell]) continue;
            if (c > 0)
                neighbours['W'][cell] = neighbours['W'][cell - 1] + 1;
            if (r > 0)
                neighbours['N'][cell] = neighbours['N'][cell - g_cols] + 1;
        }
    for (int64_t r = g_rows - 1; r >= 0; --r)
        for (int64_t c = g_cols - 1; c >= 0; --c) {
            const int64_t cell = r * g_cols + c;
            if (!g_board[cell]) continue;
            if (c < g_cols - 1)
                neighbours['E'][cell] = neighbours['E'][cell + 1] + 1;
            if (r < g_rows - 1)
                neighbours['S'][cell] = neighbours['S'][cell + g_cols] + 1;
        }

    uint64_t ret{0};
    for (int pos = 0; pos < size; ++pos) {
        if (!g_board[pos]) continue;
        for (const auto& dir : {"NE", "NW", "SE", "SW"}) {
            const int vertical_neighbours = neighbours[dir[0]][pos];
            if (vertical_neighbours < 2) continue;
            const int horizontal_neighbours = neighbours[dir[1]][pos];
            if (horizontal_neighbours < 2) continue;
            ret += std::min(vertical_neighbours / 2, horizontal_neighbours) - 1;
            ret += std::min(horizontal_neighbours / 2, vertical_neighbours) - 1;
        }
    }

    std::cout << ret;
}

static void solve_set1() {
    std::cin >> g_rows >> g_cols;
    int size = g_rows * g_cols;
    g_board.resize(size);
    for (int i = 0; i < size; ++i)
        std::cin >> g_board[i];

    uint64_t ret{0};
    size -= g_cols;
    for (int pos = 0; pos < size; ++pos) {
        if (!g_board[pos]) continue;
        for (const char dir : {'E', 'W', 'S'}) {
            int head{pos}, hcn{1};
            std::vector<std::pair<int, int>> midpoints; // {angle vertex, head}
            while (true) {
                const int next = advance(head, dir);
                if (next == -1 || !g_board[next]) break;
                head = next;
                midpoints.emplace_back(head, ++hcn);
            }
            if (midpoints.empty())
                continue;

            for (const auto& [mid, head_count] : midpoints)
                for (const char dir2 : {'E', 'W', 'S'}) {
                    if (dir2 == dir || (dir != 'S' && dir2 != 'S')) continue;
                    int tail{mid}, tail_count{1};
                    while (true) {
                        const int next = advance(tail, dir2);
                        if (next == -1 || !g_board[next]) break;
                        tail = next;
                        ++tail_count;
                        if (head_count == 2 * tail_count || tail_count == 2 * head_count)
                            ++ret;
                    }
                }
        }
    }

    std::cout << ret;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 lshaped_plots.cpp -o lshaped_plots.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address lshaped_plots.cpp -o lshaped_plots.o

Run:
lshaped_plots.exe < lshaped_plots.in

Input:

2
4 3
1 0 0
1 0 1
1 0 0
1 1 0
6 4
1 0 0 0
1 0 0 1
1 1 1 1
1 0 1 0
1 0 1 0
1 1 1 0

Output:

Case #1: 1
Case #2: 9

*/

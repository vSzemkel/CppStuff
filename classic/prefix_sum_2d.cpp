
#include <iostream>
#include <vector>

// Prefix sum for 2D matrix

int64_t g_rows, g_cols;
std::vector<int64_t> g_input, g_partial;

const auto rc  = [](const int64_t row, const int64_t col) noexcept { return row * g_cols + col; };

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int N; std::cin >> N >> g_rows >> g_cols;
    g_input.resize(g_rows * g_cols);
    g_partial.resize(g_rows * g_cols);
    for (auto& cell : g_input)
        std::cin >> cell;

    // pre-compute g_partial
    for (int c = 0; c < g_cols; ++c)
        g_partial[rc(0, c)] = g_input[rc(0, c)];
    for (int r = 1; r < g_rows; ++r)
        for (int c = 0; c < g_cols; ++c)
            g_partial[rc(r, c)] = g_input[rc(r, c)] + g_partial[rc(r - 1, c)];
    for (int r = 0; r < g_rows; ++r)
        for (int c = 1; c < g_cols; ++c)
            g_partial[rc(r, c)] += g_partial[rc(r, c - 1)];

    for (int g = 1; g <= N; ++g) {
        int rt, rb, cl, cr; std::cin >> rt >> cl >> rb >> cr;
        int64_t ret = g_partial[rc(rb, cr)];
        if (rt > 0) ret -= g_partial[rc(rt - 1, cr)];
        if (cl > 0) ret -= g_partial[rc(rb, cl - 1)];
        if (rt > 0 && cl > 0) ret += g_partial[rc(rt - 1, cl - 1)];
        std::cout << "Case #" << g << ": " << ret << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 partial_sum_2d.cpp -o partial_sum_2d.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address partial_sum_2d.cpp -o partial_sum_2d.o

Run:
partial_sum_2d.exe < partial_sum_2d.in

Input:

5 5 6
7 5 4 8 0 4
3 2 5 3 1 8
2 5 4 0 6 6
4 8 9 8 2 4
6 2 0 0 7 5
0 0 1 2
1 2 4 5
2 2 3 3
2 1 4 2
4 0 4 5

Output:

Case #0: 26
Case #1: 82
Case #2: 41
Case #3: 42
Case #4: 20

*/

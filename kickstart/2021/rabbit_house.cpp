
#include <iostream>
#include <numeric>
#include <vector>

// Rabbit House
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000436140/000000000068cb14#problem

int64_t g_rows, g_cols, g_size, g_ret;
std::vector<int64_t> g_board, g_suspects, g_next;

const auto col = [](const int64_t pos) noexcept { return pos % g_cols; };
const auto row = [](const int64_t pos) noexcept { return pos / g_cols; };

static void sanitize(const int pos)
{
    const int E = pos + 1;
    if (col(pos) < g_cols - 1 && g_board[pos] > g_board[E] + 1) {
        const int64_t correction = g_board[pos] - g_board[E] - 1;
        g_ret += correction;
        g_board[E] += correction;
        g_next.push_back(E);
    }
    const int W = pos - 1;
    if (col(pos) > 0 && g_board[pos] > g_board[W] + 1) {
        const int64_t correction = g_board[pos] - g_board[W] - 1;
        g_ret += correction;
        g_board[W] += correction;
        g_next.push_back(W);
    }
    const int N = pos - g_cols;
    if (row(pos) > 0 && g_board[pos] > g_board[N] + 1) {
        const int64_t correction = g_board[pos] - g_board[N] - 1;
        g_ret += correction;
        g_board[N] += correction;
        g_next.push_back(N);
    }
    const int S = pos + g_cols;
    if (row(pos) < g_rows - 1 && g_board[pos] > g_board[S] + 1) {
        const int64_t correction = g_board[pos] - g_board[S] - 1;
        g_ret += correction;
        g_board[S] += correction;
        g_next.push_back(S);
    }
}

static void solve()
{
    std::cin >> g_rows >> g_cols;
    const int size = g_rows * g_cols;
    g_board.resize(size);
    g_suspects.resize(size);
    for (auto& cell : g_board)
        std::cin >> cell;

    g_ret = 0;
    std::iota(g_suspects.begin(), g_suspects.end(), 0);
    while (!g_suspects.empty()) {
        for (const int pos : g_suspects)
            sanitize(pos);
        g_suspects = std::move(g_next);
        g_next.clear();
    }

    std::cout << g_ret;
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 rabbit_house.cpp -o rabbit_house.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address rabbit_house.cpp -o rabbit_house.o

Run:
py.exe interactive_runner.py py.exe rabbit_house_testing_tool.py 1 -- rabbit_house.exe
rabbit_house.exe < rabbit_house.in

Input:

3
1 3
3 4 3
1 3
3 0 0
3 3
0 0 0
0 2 0
0 0 0

Output:

Case #1: 0
Case #2: 3
Case #3: 4

*/

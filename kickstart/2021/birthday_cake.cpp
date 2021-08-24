
#include <algorithm>
#include <iostream>

// Birthday cake
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000043585c/000000000085a285

int64_t R, C, K;

static int64_t cost(const int64_t len) {
    return (len + K - 1) / K;
}

static void solve() {
    std::cin >> R >> C >> K;
    int64_t RMIN, CMIN, RMAX, CMAX; std::cin >> RMIN >> CMIN >> RMAX >> CMAX;
    const int64_t cols = CMAX - CMIN + 1, rows = RMAX - RMIN + 1;
    const int64_t cost_w = cost(cols), cost_h = cost(rows);

    int64_t cur, ret{4LL * R * C};
    // from top
    cur = 0;
    if (CMIN > 1 && CMAX < C)
        cur = cost(RMAX);
    if (RMIN > 1)
        cur += cost_w;
    if (RMAX < R)
        cur += cost_w;
    if (CMIN > 1 || CMAX < C)
        cur += cost_h;
    ret = std::min(ret, cur);
    // from bottom
    cur = 0;
    if (CMIN > 1 && CMAX < C)
        cur = cost(R - RMIN + 1);
    if (RMIN > 1)
        cur += cost_w;
    if (RMAX < R)
        cur += cost_w;
    if (CMIN > 1 || CMAX < C)
        cur += cost_h;
    ret = std::min(ret, cur);
    // from left
    cur = 0;
    if (RMIN > 1 && RMAX < R)
        cur = cost(CMAX);
    if (CMIN > 1)
        cur += cost_h;
    if (CMAX < C)
        cur += cost_h;
    if (RMIN > 1 || RMAX < R)
        cur += cost_w;
    ret = std::min(ret, cur);
    // from right
    cur = 0;
    if (RMIN > 1 && RMAX < R)
        cur = cost(C - CMIN + 1);
    if (CMIN > 1)
        cur += cost_h;
    if (CMAX < C)
        cur += cost_h;
    if (RMIN > 1 || RMAX < R)
        cur += cost_w;
    ret = std::min(ret, cur);

    // extracting inner grid point requires 4 or 3 cuts
    // we minimize number of cuts ending in inner points
    // cut squares K * K and then extract single elements
    const auto inner_cols = (cols - 1) / K;
    const auto inner_rows = (rows - 1) / K;
    ret += inner_cols * inner_rows + rows * cols - 1;

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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 birthday_cake.cpp -o birthday_cake.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address birthday_cake.cpp -o birthday_cake

Run:
birthday_cake.exe < birthday_cake.in

Input:


Output:


*/

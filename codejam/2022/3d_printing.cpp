
#include <array>
#include <iostream>
#include <numeric>

// 3D Printing
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000876ff1/0000000000a4672b

constexpr const int SUM = 1'000'000;

static void solve() {
    std::array<int, 4> printers[3];
    for (auto& p : printers)
        std::cin >> p[0] >> p[1] >> p[2] >> p[3];

    std::array<int, 4> minp;
    for (int i = 0; i < 4; ++i)
        minp[i] = std::min(std::min(printers[0][i], printers[1][i]), printers[2][i]);

    if (std::accumulate(minp.begin(), minp.end(), 0) < SUM)
        std::cout << "IMPOSSIBLE";
    else {
        int used{0};
        std::array<int, 4> color{};
        for (int i = 0; i < 4; ++i)
            if (used + minp[i] <= SUM) {
                used += minp[i];
                color[i] = minp[i];
            } else {
                color[i] = SUM - used;
                break;
            }

        std::cout << color[0] << ' ' <<color[1] << ' ' << color[2] << ' ' << color[3];
    }
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
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 3d_printing.cpp -o 3d_printing.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address 3d_printing.cpp -o 3d_printing

Run:
3d_printing.exe < 3d_printing.in

Input:

3
300000 200000 300000 500000
300000 200000 500000 300000
300000 500000 300000 200000
1000000 1000000 0 0
0 1000000 1000000 1000000
999999 999999 999999 999999
768763 148041 178147 984173
699508 515362 534729 714381
949704 625054 946212 951187

Output:

Case #1: 300000 200000 300000 200000
Case #2: IMPOSSIBLE
Case #3: 699508 148041 152451 0

*/

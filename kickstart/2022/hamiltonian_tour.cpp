
#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdlib>
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
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <vector>

// Hamiltonian Tour
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008caa74/0000000000acf318


static void solve() {
    int R, C;
    std::cin >> R >> C;
    const int SZ = R * C;
    std::vector<bool> seen(SZ);
    for (int i = 0; i < SZ; ++i) {
        char c; std::cin >> c;
        seen[i] = c == '#';
    }

    const int dr[4] = {1, 0, -1, 0}; // row
    const int dc[4] = {0, 1, 0, -1}; // col
    std::vector<int> stack(1, 0), order;
    seen[0] = true;
    while (!stack.empty()) {
        int next{-1};
        const div_t rc = div(stack.back(), C);
        const int& r = rc.quot, c = rc.rem;
        for (int i = 0; i < 4; ++i) {
            const int s = r + dr[i], d = c + dc[i];
            if (~s && s < R && ~d && d < C && !seen[s * C + d]) {
                next = s * C + d;
                seen[next] = true;
                break;
            }
        }

        order.push_back(stack.back());
        if (~next)
            stack.push_back(next);
        else
            stack.pop_back();
    }

    if (!std::all_of(seen.begin(), seen.end(), [](const bool b){ return b; })) {
        std::cout << "IMPOSSIBLE";
        return;
    }

    const int size = int(order.size());
    if (size == 1) {
        std::cout << "SENW";
        return;
    }

    std::string path;
    int pr = order[0] / C, pc = order[0] % C;
    for (int i = 1; i < size; ++i) {
        const div_t rc = div(order[i], C);
        const int& r = rc.quot, c = rc.rem;
        const int dr = r - pr, dc = c - pc;
        if (dr == 1 && dc == 0)
            path += 'S';
        if (dr == 0 && dc == 1)
            path += 'E';
        if (dr == -1 && dc == 0)
            path += 'N';
        if (dr == 0 && dc == -1)
            path += 'W';
        pr = r; pc = c;
    }

    std::cout << path;
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
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 hamiltonian_tour.cpp -o hamiltonian_tour.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address hamiltonian_tour.cpp -o hamiltonian_tour

Run:
hamiltonian_tour.exe < hamiltonian_tour.in

Input:

6
1 1
*
2 2
**
*#
3 4
****
*#*#
****
3 1
*
*
#
1 3
*#*
3 4
**#*
**#*
****

Output:


*/

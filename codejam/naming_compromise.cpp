
#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

// Naming compromise
// https://codingcompetitions.withgoogle.com/codejam/round/000000000019ff7e/00000000003774db

enum class move_t : uint8_t {
    left,
    top,
    diag,
    init
};

using cell_t = std::pair<int, move_t>;

std::string solve() {
    std::string s1, s2; std::cin >> s1 >> s2;
    const int size1 = s1.size();
    const int size2 = s2.size();
    std::vector<std::vector<cell_t>> cost(size1 + 1, std::vector<cell_t>(size2 + 1));
    for (int r = 1; r <= size1; ++r) cost[r][0] = {r, move_t::init};
    for (int c = 1; c <= size2; ++c) cost[0][c] = {c, move_t::init};
    for (int r = 1; r <= size1; ++r)
        for (int c = 1; c <= size2; ++c) {
            const int substitution_cost = (s1[r - 1] == s2[c - 1] ? 0 : 1);
            std::array<cell_t, 3> possible {
                cell_t{cost[r - 1][c].first + 1, move_t::top},
                cell_t{cost[r][c - 1].first + 1, move_t::left},
                cell_t{cost[r - 1][c - 1].first + substitution_cost, move_t::diag},
            };
            cost[r][c] = *std::min_element(possible.begin(), possible.end());
        }

    int r{size1}, c{size2};
    const int compromise = (cost[size1][size2].first + 1) / 2;
    while (true) {
        const auto& cell = cost[r][c];
        if (r == 0)
            return s2.substr(compromise - cell.first + c);
        if (c == 0)
            return s1.substr(compromise - cell.first + r);
        if (cell.first == compromise)
            return s1.substr(0, r) + s2.substr(c);
        switch (cell.second) {
            case move_t::top: --r; break;
            case move_t::left: --c; break;
            case move_t::diag: --r, --c; break;
            default: break;
        }
    }

    return "";
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g)
        std::cout << "Case #" << g << ": " << solve() << "\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 naming_compromise.cpp -o naming_compromise.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 naming_compromise.cpp -o naming_compromise.o

Run:
naming_compromise.exe < naming_compromise.in

Input:

8
A CCCBA
ABCD ABCDEFGHIJ
XYZZY ZZYZX
AAA AAABB
AAABBAA AAAAAAA
Y Z
YYXXYZ ZYYXXY
XZXZXZ YZ

Output:

Case #1: CBA
Case #2: ABCDHIJ
Case #3: XYZZYZX
Case #4: AAAB
Case #5: AAABAAA
Case #6: Y
Case #7: YYXXY
Case #8: XZX

*/

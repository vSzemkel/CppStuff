
#include <assert.h>
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>

// Pascal Walk
// https://codingcompetitions.withgoogle.com/codejam/round/000000000019fd74/00000000002b1353

constexpr int g_max_path = 500;

void solve() {
    int size; std::cin >> size;
    const int max_rows_gap = floor(log2(size));
    std::vector<std::pair<int, int>> ret{{1, 1}};

    bool left_ended{true};
    int score{1}, len{1}, row{2};
    int target = (size - max_rows_gap) >> 1;
    while (target > 0) {
        if (target % 2 == 0) {
            ++len;
            ++score;
            ret.emplace_back(row, left_ended ? 1 : row);
        } else {
            len += row;
            score += (1 << (row - 1));
            for (int c = 1; c <= row; ++c)
                ret.emplace_back(row, left_ended ? c : row - c + 1);
            left_ended = !left_ended;
        }
        ++row;
        target >>= 1;
    }

    while (score < size) {
        ++len;
        ++score;
        ret.emplace_back(row, left_ended ? 1 : row);
        ++row;
    }

    assert(len <= g_max_path);
    for (const auto& p : ret)
        std::cout << p.first << " " << p.second << "\n";
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ":\n"; solve(); std::cout << "\n";
    }
}

/*
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 pascal_walk.cpp -o pascal_walk.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 pascal_walk.cpp -o pascal_walk.o
pascal_walk.exe < pascal_walk.in

Input:


Output:


*/

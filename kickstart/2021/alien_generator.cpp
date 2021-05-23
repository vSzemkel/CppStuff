
#include <cmath>
#include <iostream>

// Alien generator
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000435c44/00000000007ec1cb


static void solve() {
    int64_t G; std::cin >> G;
    const auto MAX = int(std::sqrt(2 * G)) + 1;

    int ret{1};
    for (int d = 2; d < MAX; ++d) {
        if ((d & 1) > 0 && G % d == 0)
            ++ret;
        if ((d & 1) == 0 && G % (d / 2) == 0 && ((G / (d / 2)) & 1) > 0)
            ++ret;
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 alien_generator.cpp -o alien_generator.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address alien_generator.cpp -o alien_generator

Run:
alien_generator.exe < alien_generator.in

Input:

3
10
125
1001

Output:

Case #1: 2
Case #2: 4
Case #3: 8

*/

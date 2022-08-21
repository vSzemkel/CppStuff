
#include <iostream>

// Coloring Game
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb0f5/0000000000ba856a

static void solve() {
    int N;
    std::cin >> N;
    int ans = N / 5;
    if (N % 5)
        ++ans;
    std::cout << ans;
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 coloring_game.cpp -o coloring_game.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address coloring_game.cpp -o coloring_game

Run:
coloring_game.exe < coloring_game.in

Input:

3
4
5
6

Output:

Case #1: 1
Case #2: 1
Case #3: 2

*/

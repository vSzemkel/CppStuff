
#include <array>
#include <iostream>
#include <unordered_map>

// Arithmetic Square
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000004361e3/000000000082b813


static void solve() {
    std::array<std::array<int, 3>, 3> matrix;
    for (int c = 0; c < 3; ++c) std::cin >> matrix[0][c];
    std::cin >> matrix[1][0]; std::cin >> matrix[1][2];
    for (int c = 0; c < 3; ++c) std::cin >> matrix[2][c];


    // not choice dependent
    int ans{0}, fix{0};
    if ((matrix[0][0] + matrix[0][2]) == 2 * matrix[0][1]) ++fix;
    if ((matrix[2][0] + matrix[2][2]) == 2 * matrix[2][1]) ++fix;
    if ((matrix[0][0] + matrix[2][0]) == 2 * matrix[1][0]) ++fix;
    if ((matrix[0][2] + matrix[2][2]) == 2 * matrix[1][2]) ++fix;

    // choice dependent
    std::unordered_map<int, int> scores;
    const int row = matrix[1][0] + matrix[1][2];
    if (row % 2 == 0) ++scores[row / 2];
    const int col = matrix[0][1] + matrix[2][1];
    if (col % 2 == 0) ++scores[col / 2];
    const int d1 = matrix[0][0] + matrix[2][2];
    if (d1 % 2 == 0) ++scores[d1 / 2];
    const int d2 = matrix[0][2] + matrix[2][0];
    if (d2 % 2 == 0) ++scores[d2 / 2];

    for (const auto& s : scores)
        if (s.second > ans)
            ans = s.second;

    std::cout << ans + fix;
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 arithmetic_square.cpp -o arithmetic_square.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address arithmetic_square.cpp -o arithmetic_square

Run:
py.exe interactive_runner.py py.exe arithmetic_square_testing_tool.py 1 -- arithmetic_square.exe
arithmetic_square.exe < arithmetic_square.in

Input:

3
3 4 11
10 9
-1 6 7
4 1 6
3 5
2 5 6
9 9 9
9 9
9 9 9

Output:

Case #1: 4
Case #2: 3
Case #3: 8

*/

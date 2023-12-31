
#include <array>
#include <iostream>

// Backtracking queens
// On NxN chessboard placeN queens not attacking each other
// This can be effectively solved with SAT

constexpr const int MAX = 30;

std::array<int, MAX> solution; // row => col mapping
std::array<int, MAX> index;    // col => row mapping
std::array<bool, 2 * MAX - 1> ne_diag, se_diag; // taken diagonals bitmap 

/**
 * @brief Intuitive approach to backing data structures.
 * Generates row -> col solution
 */
static bool solve(const int row, const int target) {
    if (row == target) {
        std::cout << '[';
        for (int i = 0; i < target - 1; ++i)
            std::cout << solution[i] << ", ";
        std::cout << solution[target - 1] << ']';
        return true;
    }

    for (int col = 0; col < target; ++col) {
        if (index[col] >= 0)
            continue;
        bool colision{};
        int r = row - 1;
        int c = col - 1;
        while (r >= 0 && c >= 0) {
            if (solution[r] == c) {
                colision = true;
                break;
            }
            --r; --c;
        }
        if (colision)
            continue;
        r = row - 1;
        c = col + 1;
        while (r >= 0 && c < target) {
            if (solution[r] == c) {
                colision = true;
                break;
            }
            --r; ++c;
        }
        if (colision)
            continue;

        solution[row] = col;
        index[col] = row;
        if (solve(row + 1, target))
            return true;
        solution[row] = -1;
        index[col] = -1;
    }

    if (row == 0)
        std::cout << "IMPOSSIBLE";
    return false;
}

/**
 * @brief Alternative approach to backing data structures.
 * Faster solution, generates col -> row mapping
 */
static bool solve2(const int row, const int target) {
    if (row == target) {
        const auto tmp = solution;
        for (int col = 0; col < target; ++col)
            solution[tmp[col]] = col;
        std::cout << '[';
        for (int i = 0; i < target - 1; ++i)
            std::cout << solution[i] << ", ";
        std::cout << solution[target - 1] << ']';
        return true;
    }

    for (int col = 0; col < target; ++col) {
        const int ne = row + col;
        const int se = target - 1 + row - col;
        if (solution[col] >= 0 || ne_diag[ne] || se_diag[se])
            continue;

        solution[col] = row;
        ne_diag [ne] = se_diag[se] = true;
        if (solve2(row + 1, target))
            return true;
        solution[col] = -1;
        ne_diag[ne] = se_diag[se] = false;
    }

    if (row == 0)
        std::cout << "IMPOSSIBLE";
    return false;
}

int main(int, char**)
{
    for (int n = 1; n <= MAX; ++n) {
        index.fill(-1);
        solution.fill(-1);
        std::cout << n << ": "; solve(0, n); std::cout << '\n';
    }

    for (int n = 1; n <= MAX; ++n) {
        solution.fill(-1);
        ne_diag.fill(false);
        se_diag.fill(false);
        std::cout << n << ": "; solve2(0, n); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 backtracking_queens.cpp -o backtracking_queens.exe

Output:

1: [0]
2: IMPOSSIBLE
3: IMPOSSIBLE
4: [1, 3, 0, 2]
5: [0, 2, 4, 1, 3]
6: [1, 3, 5, 0, 2, 4]
7: [0, 2, 4, 6, 1, 3, 5]
8: [0, 4, 7, 5, 2, 6, 1, 3]
...
29: [0, 2, 4, 1, 3, 8, 10, 12, 14, 5, 19, 23, 25, 20, 28, 26, 24, 27, 7, 11, 6, 15, 9, 16, 21, 13, 17, 22, 18]
30: [0, 2, 4, 1, 3, 8, 10, 12, 14, 6, 22, 25, 27, 24, 21, 23, 29, 26, 28, 15, 11, 9, 7, 5, 17, 19, 16, 13, 20, 18]
*/


#include <array>
#include <iostream>

// Word Search II
// https://leetcode.com/problems/n-queens-ii

// Fastest than this solution is to enumerate 2n-1 diagonals with r+c and n-1+r-c
// int solve(vector<bool>& col, vector<bool>& diag, vector<bool>& anti_diag, int row) {
//     int n = size(col), count = 0;
//     if (row == n) return 1;
//     for (int column = 0; column < n; ++column)
//         if (!col[column] && !diag[row + column] && !anti_diag[row - column + n - 1]) {
//             col[column] = diag[row + column] = anti_diag[row - column + n - 1] = true;
//             count += solve(col, diag, anti_diag, row + 1); 
//             col[column] = diag[row + column] = anti_diag[row - column + n - 1] = false; 
//         }
//     return count;
// }


constexpr const int MAX = 29;

int N;
int64_t ANS;
std::array<int, MAX> solution; // row => col mapping
std::array<int, MAX> index;    // col => row mapping

void solve(const int row) {
    for (int col = 0; col < N; ++col) {
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
        while (r >= 0 && c < N) {
            if (solution[r] == c) {
                colision = true;
                break;
            }
            --r; ++c;
        }
        if (colision)
            continue;

        if (row == N - 1)
            ++ANS;
        else {
            solution[row] = col;
            index[col] = row;
            solve(row + 1);
            solution[row] = -1;
            index[col] = -1;
        }
    }
}

int totalNQueens(int n) {
    N = n;
    ANS = 0;
    solution.fill(-1);
    index.fill(-1);
    solve(0);
    return ANS;
}

static void io_handler() {
    std::cin >> N;
    std::cout << N << ": " << totalNQueens(N);
}

int main(int, char**)
{
    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; io_handler(); std::cout << '\n';
    }
}
/*

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 n-queens_ii.cpp -o n-queens_ii.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address n-queens_ii.cpp -o n_queens-ii

Run:
n-queens_ii.exe < n-queens_ii.in

Input:

3
5
10
15

Output:

Case #1: 5: 10
Case #2: 10: 724
Case #3: 15: 2279184

*/

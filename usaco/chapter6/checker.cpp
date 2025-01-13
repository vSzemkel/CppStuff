/*
ID: marcin.8
LANG: C++
TASK: checker
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=iuNOXJ9T5Ya&S=checker
*/

#include <array>
#include <fstream>

std::ifstream task_in("checker.in");
std::ofstream task_out("checker.out");

constexpr const int SOL = 3;
constexpr const int MAX = 30;

std::array<int, MAX> solution;                      // row => col mapping
std::array<bool, MAX> col_taken{};                  // col is taken
std::array<bool, 2 * MAX - 1> ne_diag{}, se_diag{}; // taken diagonals bitmap

int N, ans{}, sol{};

void checker(const int row)
{
    if (row == N) {
        if (++sol <= SOL) {
            for (int i = 0; i < N - 1; ++i)
                task_out << solution[i] + 1 << ' ';
            task_out << solution[N - 1] + 1 << '\n';
        }
        ++ans;
    }

    for (int col = 0; col < N; ++col) {
        const int ne = row + col;
        const int se = N - 1 - row + col;
        if (col_taken[col] || ne_diag[ne] || se_diag[se])
            continue;

        solution[row] = col;
        col_taken[col] = ne_diag[ne] = se_diag[se] = true;
        checker(row + 1);
        col_taken[col] = ne_diag[ne] = se_diag[se] = false;
    }
}

int main(int, char**)
{
    task_in >> N;

    solution.fill(-1);
    checker(0);
    task_out << ans << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 checker.cpp -o checker.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address checker.cpp -o checker

Run:
checker.exe && type checker.out

Input:

13

Output:

1 3 5 2 9 12 10 13 4 6 8 11 7
1 3 5 7 9 11 13 2 4 6 8 10 12
1 3 5 7 12 10 13 6 4 2 8 11 9
73712

*/

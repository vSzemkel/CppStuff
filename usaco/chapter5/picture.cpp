/*
ID: marcin.8
LANG: C++
TASK: picture
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=RVlGy5XNBIY&S=picture
*/

#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>

std::ifstream task_in("picture.in");
std::ofstream task_out("picture.out");

static constexpr int offset = 10001;
static constexpr int size = 2 * offset + 1;
std::array<std::array<bool, size>, size> canvas{};
std::array<bool, size> row_taken{}, col_taken{};
int main(int, char**)
{
    int N;
    for (task_in >> N; N; --N) {
        int lx, ly, ux, uy;
        task_in >> lx >> ly >> ux >> uy;
        lx += offset;
        ly += offset;
        ux += offset;
        uy += offset;
        assert(lx <= ux && ly <= uy);
        std::fill(row_taken.data() + ly, row_taken.data() + uy, true);
        std::fill(col_taken.data() + lx, col_taken.data() + ux, true);
        for (int r = ly; r < uy; ++r)
            for (int c = lx; c < ux; ++c)
                canvas[r][c] = true;
    }

    int ans{};
    auto& prev_row = canvas[0];
    for (int r = 1; r < size; ++r) {
        const auto& cur_row = canvas[r];
        if (row_taken[r] || row_taken[r - 1]) {
            for (int c = 0; c < size; ++c)
                ans += prev_row[c] ^ cur_row[c];
        }

        prev_row = cur_row;
    }

    for (int cur_col = 1; cur_col < size; ++cur_col)
        if (col_taken[cur_col - 1] || col_taken[cur_col])
            for (const auto& cur_row : canvas)
                ans += cur_row[cur_col - 1] ^ cur_row[cur_col];

    task_out << ans << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 picture.cpp -o picture.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address picture.cpp -o picture

Run:
picture.exe && type picture.out

Input:

7
-15 0 5 10
-5 8 20 25
15 -4 24 14
0 -6 16 4
2 15 10 22
30 10 36 20
34 0 40 16 

Output:

228

*/

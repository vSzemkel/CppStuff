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
#include <vector>

std::ifstream task_in("picture.in");
std::ofstream task_out("picture.out");

static constexpr int MAX = 10000;

int main(int, char**)
{
    int N, mx{MAX}, Mx{-MAX}, my{MAX}, My{-MAX};
    std::vector<std::array<int, 4>> pictures;
    for (task_in >> N; N; --N) {
        int lx, ly, ux, uy;
        task_in >> lx >> ly >> ux >> uy;
        assert(lx <= ux && ly <= uy);
        mx = std::min(mx, lx);
        Mx = std::max(Mx, ux);
        my = std::min(my, ly);
        My = std::max(My, uy);
        pictures.push_back({lx, ly, ux, uy});
    }

    const int size_x = Mx - mx + 2;
    const int size_y = My - my + 2;
    const int offset_x = (mx <= 0) ? -mx + 1 : 0;
    const int offset_y = (my <= 0) ? -my + 1 : 0;

    std::vector<std::vector<bool>> canvas(size_y, std::vector<bool>(size_x));
    std::vector<bool> row_taken(size_y), col_taken(size_x);
    for (auto& [lx, ly, ux, uy] : pictures) {
        lx += offset_x;
        ly += offset_y;
        ux += offset_x;
        uy += offset_y;
        for (int x = lx; x < ux; ++x)
            col_taken[x] = true;
        for (int y = ly; y < uy; ++y)
            row_taken[y] = true;
        for (int r = ly; r < uy; ++r)
            for (int c = lx; c < ux; ++c)
                canvas[r][c] = true;
    }

    int ans{};
    auto& prev_row = canvas[0];
    for (int r = 1; r < size_y; ++r) {
        const auto& cur_row = canvas[r];
        if (row_taken[r] || row_taken[r - 1]) {
            for (int c = 0; c < size_x; ++c)
                ans += prev_row[c] ^ cur_row[c];
        }

        prev_row = cur_row;
    }

    for (int cur_col = 1; cur_col < size_x; ++cur_col)
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

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
#include <set>
#include <vector>

std::ifstream task_in("picture.in");
std::ofstream task_out("picture.out");

static constexpr int MAX = 10000;

using rect_t = std::array<int, 4>;
const auto left_check = [](const rect_t& p1, const rect_t& p2) {
    return p1[0] < p2[0];
};
const auto right_check = [](const rect_t& p1, const rect_t& p2) {
    return p1[2] < p2[2];
};
std::multiset<rect_t, decltype(right_check)> window(right_check);

void get_strip(std::vector<bool>& strip, int col)
{
    while (!window.empty() && (*window.begin())[2] <= col)
        window.erase(window.begin());

    std::fill(strip.begin(), strip.end(), false);
    for (const auto& r : window)
        std::fill(strip.begin() + r[1], strip.begin() + r[3], true);
}

int main(int, char**)
{
    int N, mx{MAX}, Mx{-MAX}, my{MAX}, My{-MAX};
    task_in >> N;
    std::vector<rect_t> pictures;
    pictures.reserve(N);
    for (int z = N; z; --z) {
        int lx, ly, ux, uy;
        task_in >> lx >> ly >> ux >> uy;
        assert(lx <= ux && ly <= uy);
        mx = std::min(mx, lx);
        Mx = std::max(Mx, ux);
        my = std::min(my, ly);
        My = std::max(My, uy);
        pictures.push_back({lx, ly, ux, uy});
    }

    int size_x = Mx - mx + 2;
    int size_y = My - my + 2;
    const int offset_x = (mx <= 0) ? -mx + 1 : 0;
    const int offset_y = (my <= 0) ? -my + 1 : 0;

    for (auto& [lx, ly, ux, uy] : pictures) {
        lx += offset_x;
        ly += offset_y;
        ux += offset_x;
        uy += offset_y;
    }

    int ans{};
    for (int z = 2; true; --z) {
        std::vector<bool> prev_strip(size_y), cur_strip(size_y);
        std::sort(pictures.begin(), pictures.end(), left_check);

        int pi{};
        while (pi < N && pictures[pi][0] == 0)
            window.insert(pictures[pi++]);
        for (int i = 1; i < size_x; ++i) {
            while (pi < N && pictures[pi][0] == i)
                window.insert(pictures[pi++]);
            get_strip(cur_strip, i);
            for (int r = 0; r < size_y; ++r)
                ans += prev_strip[r] ^ cur_strip[r];
            std::swap(prev_strip, cur_strip);
        }

        if (z == 1)
            break;

        std::swap(size_x, size_y);
        for (auto& [lx, ly, ux, uy] : pictures) {
            std::swap(lx, ly);
            std::swap(ux, uy);
        }
    }

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

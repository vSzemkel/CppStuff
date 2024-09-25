/*
ID: marcin.8
LANG: C++
TASK: rectbarn
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=MzuPZTHvs0f&S=rectbarn
*/

#include <algorithm>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

std::ifstream task_in("rectbarn.in");
std::ofstream task_out("rectbarn.out");

int R, C;
const char GOOD{' '};
std::vector<std::string> field;

int max_rect_in_histogram(const std::vector<int>& hist)
{
    int ans{};
    std::vector<std::pair<int, int>> stack; // {left, height}
    stack.emplace_back(0, hist[0]);
    for (int c = 1; c <= C; ++c) {
        auto left{c};
        const int h = hist[c];

        while (!stack.empty() && stack.back().second > h) {
            const auto can = (c - stack.back().first) * stack.back().second;
            ans = std::max(ans, can);
            left = stack.back().first;
            stack.pop_back();
        }

        if (stack.empty() || stack.back().second < h)
            stack.emplace_back(left, h);
    }

    return ans;
}

int main(int, char**)
{
    int b;
    task_in >> R >> C >> b;
    field.assign(R, std::string(C, GOOD));
    for (; b; --b) {
        int r, c;
        task_in >> r >> c;
        field[--r][--c] = '#';
    }

    int ans{};
    std::vector<int> histogram(C + 1);
    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c)
            if (field[r][c] == GOOD)
                ++histogram[c];
            else
                histogram[c] = 0;
        ans = std::max(ans, max_rect_in_histogram(histogram));
    }

    task_out << ans << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 rectbarn.cpp -o rectbarn.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address rectbarn.cpp -o rectbarn

Run:
rectbarn.exe && type rectbarn.out

Input:

16 16 10
8 1
10 10
16 14
14 1
12 15
1 9
7 8
8 16
8 12
10 16

Output:

96

*/

/*
ID: marcin.8
LANG: C++
TASK: transform
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=voC05iclk5w&S=transform
*/

#include <iostream>
#include <fstream>
#include <vector>

std::ifstream task_in("transform.in");
std::ofstream task_out("transform.out");

auto rotate90(const std::vector<std::vector<char>>& src, const int size)
{
    std::vector<std::vector<char>> ret(size, std::vector<char>(size));
    for (int r = 0; r < size; ++r)
        for (int c = 0; c < size; ++c)
            ret[c][size - r - 1] = src[r][c];
    return ret;
}

auto mirror_h(const std::vector<std::vector<char>>& src, const int size)
{
    auto ret = src;
    for (int c = 0; c < size / 2; ++c)
        for (int r = 0; r < size; ++r)
            std::swap(ret[r][c], ret[r][size - c - 1]);
    return ret;
}

int solve()
{
    int size; task_in >> size;
    std::vector<std::vector<char>> src(size, std::vector<char>(size));
    auto dst = src;
    for (int r = 0; r < size; ++r)
        for (int c = 0; c < size; ++c)
            task_in >> src[r][c];
    for (int r = 0; r < size; ++r)
        for (int c = 0; c < size; ++c)
            task_in >> dst[r][c];

    const auto r90 = rotate90(src, size);
    const auto r180 = rotate90(r90, size);
    const auto r270 = rotate90(r180, size);
    const auto mh = mirror_h(src, size);

    if (r90 == dst) return 1;
    if (r180 == dst) return 2;
    if (r270 == dst) return 3;
    if (mh == dst) return 4;
    if (rotate90(mh, size) == dst
     || rotate90(rotate90(mh, size), size) == dst
     || rotate90(rotate90(rotate90(mh, size), size), size) == dst) return 5;
    if (src == dst) return 6;
    return 7;
}

int main(int, char**)
{
    task_out << solve() << "\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 transform.cpp -o transform.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 transform.cpp -o transform.o

Input:

3
@-@
---
@@-
@-@
@--
--@

Output:

1

*/

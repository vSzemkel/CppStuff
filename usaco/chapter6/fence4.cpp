/*
ID: marcin.8
LANG: C++
TASK: fence4
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=szTEXMwIDAp&S=fence4
*/

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <utility>
#include <vector>

std::ifstream task_in("fence4.in");
std::ofstream task_out("fence4.out");

template <typename T = float>
struct point_t { T x, y; };

template <typename T = float>
std::istream& operator>>(std::istream& input, point_t<T>& point) {
    input >> point.x >> point.y;
    return input;
}

template <typename T = float>
using section_t = std::pair<point_t<T>, point_t<T>>;

template <typename T = float>
std::ostream& operator<<(std::ostream& output, const section_t<T>& section) {
    output << section.first.x << ' ' << section.first.y << ' ' << section.second.x << ' ' << section.second.y << '\n';
    return output;
}

int N;
point_t<int> observer;
std::vector<section_t<int>> fence;

template <typename T = float>
bool intersection_status(const section_t<T>& s1, const section_t<T>& s2)
{
    const T s1_dx = s1.second.x - s1.first.x;
    const T s1_dy = s1.second.y - s1.first.y;
    const T s2_dx = s2.second.x - s2.first.x;
    const T s2_dy = s2.second.y - s2.first.y;

    const T s = (-s1_dy * (s1.first.x - s2.first.x) + s1_dx * (s1.first.y - s2.first.y)) / (-s2_dx * s1_dy + s1_dx * s2_dy);
    if (s < 0 || 1 < s)
        return false;
    const T t = ( s2_dx * (s1.first.y - s2.first.y) - s2_dy * (s1.first.x - s2.first.x)) / (-s2_dx * s1_dy + s1_dx * s2_dy);
    if (t < 0 || 1 < t)
        return false;

    return true;
}

int main(int, char**)
{
    task_in >> N;
    fence.resize(N);
    task_in >> observer;
    point_t<int> first;
    task_in >> first;
    auto prev = first;
    for (int i = 0; i < N - 1; ++i) {
        point_t<int> next;
        task_in >> next;
        fence[i] = {prev, next};
        prev = next;
    }
    fence[N - 1] = {prev, first};

    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j)
            if (intersection_status(fence[i], fence[j])) {
                task_out << "NOFENCE\n";
                return 0;
            }
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 fence4.cpp -o fence4.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address fence4.cpp -o fence4

Run:
fence4.exe && type fence4.out

Input:

13
5 5
0 0
7 0
5 2
7 5
5 7
3 5
4 9
1 8
2 5
0 9
-2 7
0 3
-3 1

Output:

7
0 0 7 0
5 2 7 5
7 5 5 7
5 7 3 5
-2 7 0 3
0 0 -3 1
0 3 -3 1

*/

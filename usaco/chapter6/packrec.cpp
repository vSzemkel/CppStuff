/*
ID: marcin.8
LANG: C++
TASK: packrec
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=6xoencO1s1u&S=packrec
*/

#include <algorithm>
#include <array>
#include <fstream>
#include <set>
#include <utility>

std::ifstream task_in("packrec.in");
std::ofstream task_out("packrec.out");

int ans = 1e09;
static constexpr int N = 4;
std::array<std::pair<int, int>, N> rects;
std::set<std::pair<int, int>> solution;

void check(int w, int h) 
{
    const auto can = w * h;
    if (can < ans) {
        if (w > h) std::swap(w, h);
        if (can < ans)
            solution.clear();
        solution.emplace(w, h);
        ans = can;
    }
}

void analyze() 
{
    int w{}, h{};
    for (int i = 0; i < N; ++i) {
        w += rects[i].first;
        h = std::max(h, rects[i].second);
    }
    check(w, h);

    w -= rects[3].first;
    w = std::max(w, rects[3].first);
    h = rects[3].second + std::max({rects[0].second, rects[1].second, rects[2].second});
    check(w, h);

    w = std::max(rects[0].first + rects[1].first, rects[2].first) + rects[3].first;
    h = std::max(std::max(rects[0].second, rects[1].second) + rects[2].second, rects[3].second);
    check(w, h);

    w = std::max(rects[0].first, rects[1].first) + rects[2].first + rects[3].first;
    h = std::max({rects[0].second + rects[1].second, rects[2].second, rects[3].second});
    check(w, h);

    if (rects[0].first <= rects[1].first && rects[1].second <= rects[2].second && rects[3].first <= rects[1].first + rects[2].first
    && ((rects[0].first + rects[3].first <= rects[1].first + rects[2].first) || (rects[0].second + rects[1].second <= rects[2].second))) {
        w = rects[1].first + rects[2].first;
        h = std::max(rects[0].second + rects[1].second, rects[2].second + rects[3].second);
        check(w, h);
    }
}

void rotate(int order)
{
    if (order == N)
        analyze();
    else {
        rotate(order + 1);
        std::swap(rects[order].first, rects[order].second);
        rotate(order + 1);
        std::swap(rects[order].first, rects[order].second);
    }
}

int main(int, char**)
{
    for (auto& [w, h] : rects)
        task_in >> w >> h;

    std::sort(rects.begin(), rects.end());
    do {
        rotate(0);
    } while (std::next_permutation(rects.begin(), rects.end()));

    task_out << ans << '\n';
    for (auto& [w, h] : solution)
        task_out << w << ' ' << h << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 packrec.cpp -o packrec.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address packrec.cpp -o packrec

Run:
packrec.exe && type packrec.out

Input:

1 2
4 5
2 3
3 4

Output:

40
4 10
5 8

*/

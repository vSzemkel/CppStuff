/*
ID: marcin.8
LANG: C++
TASK: packrec
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=6xoencO1s1u&S=packrec
*/

#include <algorithm>
#include <array>
#include <fstream>
#include <numeric>
#include <set>
#include <utility>

std::ifstream task_in("packrec.in");
std::ofstream task_out("packrec.out");

int ans = 1e09;
static constexpr int N = 4;
std::array<int, N> perm;
std::array<std::pair<int, int>, N> rects;
std::set<std::pair<int, int>> solution;

/**
 * As rectangles have specific roles in analyse() we need all 
 * 384 permutation, even if some rectangles are identical
 */
auto rw(const int ind)
{
    return rects[perm[ind]].first;
}

auto rh(const int ind)
{
    return rects[perm[ind]].second;
}

void check(int w, int h) 
{
    const auto can = w * h;
    if (can <= ans) {
        if (w > h)
            std::swap(w, h);
        if (can < ans) {
            solution.clear();
            ans = can;
        }
        solution.emplace(w, h);
    }
}

void analyze()
{
    int w{}, h{};
    for (int i = 0; i < N; ++i) {
        w += rw(i);
        h = std::max(h, rh(i));
    }
    check(w, h);

    w = std::max(w - rw(3), rw(3));
    h = rh(3) + std::max({rh(0), rh(1), rh(2)});
    check(w, h);

    w = std::max(rw(0) + rw(1), rw(2)) + rw(3);
    h = std::max(std::max(rh(0), rh(1)) + rh(2), rh(3));
    check(w, h);

    w = std::max(rw(0), rw(1)) + rw(2) + rw(3);
    h = std::max({rh(0) + rh(1), rh(2), rh(3)});
    check(w, h);

    if (rw(0) <= rw(1) && rh(1) <= rh(2) && rw(3) <= rw(1) + rw(2)
    && ((rw(0) + rw(3) <= rw(1) + rw(2)) || (rh(0) + rh(1) <= rh(2)))) {
        w = rw(1) + rw(2);
        h = std::max(rh(0) + rh(1), rh(2) + rh(3));
        check(w, h);
    }
}

void rotate(const int order)
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

    std::iota(perm.begin(), perm.end(), 0);
    do {
        rotate(0);
    } while (std::next_permutation(perm.begin(), perm.end()));

    task_out << ans << '\n';
    for (const auto& [w, h] : solution)
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

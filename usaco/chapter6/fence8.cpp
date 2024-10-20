/*
ID: marcin.8
LANG: C++
TASK: fence8
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=CKUJn5l4XVD&S=fence8
*/

#include <algorithm>
#include <cassert>
#include <fstream>
#include <numeric>
#include <vector>

std::ifstream task_in("fence8.in");
std::ofstream task_out("fence8.out");

int K, N, W, B, total{};
std::vector<int> board, rail, spaces, prefsum;

template <typename T, typename U>
static T last_true(T lo, T hi, U f) {
    lo--;
    assert(lo <= hi); // assuming f is decreasing
    while (lo < hi) { // find last index such that f is true
        const T mid = lo + (hi - lo + 1) / 2; // this will work for negative numbers too
        f(mid) ? lo = mid : hi = mid - 1;
    }
    return lo;
}

bool distribute(int pos, int waste)
{
    if (pos < 0)
        return true;
    if (total - prefsum[B - 1] < waste)
        return false;

    const int cur = rail[pos];
    for (auto& space : spaces) {
        if (cur == space) {
            space -= cur;
            const auto ret = distribute(pos - 1, waste);
            space += cur;
            return ret;
        }
    }

    for (auto& space : spaces) {
        if (cur < space) {
            space -= cur;
            const auto nw = space < W ? waste + space : waste;
            if (distribute(pos - 1, nw))
                return true;
            space += cur;
        }
    }

    return false;
}

bool check(const int b)
{
    B = b;
    spaces = board;
    return distribute(b - 1, 0);
}

int main(int, char**)
{
    task_in >> K;
    board.resize(K);
    for (auto& b : board)
        task_in >> b;
    task_in >> N;
    rail.resize(N);
    prefsum.resize(N);
    for (auto& b : rail)
        task_in >> b;

    total = std::accumulate(board.begin(), board.end(), 0);
    std::sort(rail.begin(), rail.end());
    std::inclusive_scan(rail.begin(), rail.end(), prefsum.begin());
    W = rail.front();
    N = std::upper_bound(prefsum.begin(), prefsum.end(), total) - prefsum.begin();
    rail.resize(N);

    task_out << last_true(0, N, check) << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 fence8.cpp -o fence8.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address fence8.cpp -o fence8

Run:
fence8.exe && type fence8.out

Input:

4
30
40
50
25
10
15
16
17
18
19
20
21
25
24
30

Output:

7

*/

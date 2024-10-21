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

int K, N, shortest, required, stock;
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

/**
 * Observation1: If we can cut C rails, we can always cut C shortest rails
 * Observation2: We cut longer rails first, as finding a spece for shorters is easier
 * Observation3: We can ignore rails longer than longest board
 * Observation4: If prefix sum of sorted rails gets greater than stock, ignore rest of rails
 * Def: Waste is unused part of a board shorter than shortest required rail
 * Observation5: If sum of required rails plus waste is greater than stock, we fail
 * Observation6: Prefer zero waste. If we can use a board fully, we choose to do so
 * Observation7: If we fail after zero waste cut, we fail also in other cuts
 */
bool distribute(int pos, int waste)
{
    if (pos < 0)
        return true;
    if (stock - required < waste) // 5
        return false;

    const int cur = rail[pos]; // 1
    for (auto& space : spaces) {
        if (cur == space) { // 6
            space -= cur;
            const auto ret = distribute(pos - 1, waste);
            space += cur;
            return ret; // 7
        }
    }

    for (auto& space : spaces) {
        if (cur < space) {
            space -= cur;
            const auto nw = space < shortest ? waste + space : waste;
            if (distribute(pos - 1, nw)) // 2
                return true;
            space += cur;
        }
    }

    return false;
}

bool check(const int b)
{
    spaces = board;
    required = prefsum[b - 1];
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

    stock = std::accumulate(board.begin(), board.end(), 0);
    const auto longest_board = *std::max_element(board.begin(), board.end());
    std::sort(rail.begin(), rail.end());
    N = std::upper_bound(rail.begin(), rail.end(), longest_board) - rail.begin(); // 3
    std::inclusive_scan(rail.begin(), rail.begin() + N, prefsum.begin());
    N = std::upper_bound(prefsum.begin(), prefsum.begin() + N, stock) - prefsum.begin(); // 4
    rail.resize(N);
    shortest = rail.front();

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

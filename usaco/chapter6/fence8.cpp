/*
ID: marcin.8
LANG: C++
TASK: fence8
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=CKUJn5l4XVD&S=fence8
*/

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <filesystem>
#include <fstream>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <vector>

std::ifstream task_in("fence8.in");
std::ofstream task_out("fence8.out");

bool can;
int K, N, B, total{};
std::vector<int> board;
std::vector<int> rail, prefsum;
std::vector<int> buckets;

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

void distribute_bitmask(std::vector<int>& buckets, const int pos)
{
    if (pos == B)
        can = true;
    else {
        const int cur = rail[pos];
        for (int i = 0; !can && i < K; ++i) {
            auto& bucket = buckets[i];
            if (bucket + cur <= board[i]) {
                bucket += cur;
                distribute_bitmask(buckets, pos + 1);
                bucket -= cur;
            }
        }
    }
}

bool check(const int b)
{
    if (total < prefsum[b - 1])
        return false;

    B = b;
    can = false;
    buckets.assign(K, 0);
    distribute_bitmask(buckets, 0);
    return can;
}

int main(int, char**)
{
    task_in >> K;
    board.resize(K);
    for (auto& b : board)
        task_in >> b;
    task_in >> N;
    rail.resize(N);
    for (auto& b : rail)
        task_in >> b;

    const auto M = *std::max_element(board.begin(), board.end());
    std::sort(rail.begin(), rail.end());
    const auto nend = std::upper_bound(rail.begin(), rail.end(), M);
    rail.erase(nend, rail.end());

    N = int(rail.size());
    prefsum.resize(N);
    std::inclusive_scan(rail.begin(), rail.end(), prefsum.begin());
    total = std::accumulate(board.begin(), board.end(), 0);

    task_out << last_true(0, N, check) << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 fence8.cpp -o fence8.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address fence8.cpp -o fence8

Run:
fence8.exe && type fence8.out

Input:


Output:


*/


#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
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

// Median of Two Sorted Arrays
// https://leetcode.com/problems/median-of-two-sorted-arrays

static float solve() { // O(log(M + N))
    int M, N;
    std::cin >> M >> N;
    std::vector<int> left(M), right(N);
    for (auto& i : left) std::cin >> i;
    for (auto& i : right) std::cin >> i;

    int lsz = int(left.size());
    int rsz = int(right.size());
    const int sz = lsz + rsz;
    if (lsz > rsz) {
        std::swap(left, right);
        std::swap(lsz, rsz);
    }

    if (lsz == 0)
        return rsz % 2 ? right[rsz / 2] : (right[rsz / 2] + right[rsz / 2 - 1]) / 2.0;

    const int partition = (sz + 1) / 2;
    int lbeg{0}, rbeg{lsz};
    while (lbeg <= rbeg) {
        const auto lcut = (lbeg + rbeg) / 2;
        const auto rcut = partition - lcut;

        const auto l1 = lcut == 0 ? INT_MIN : left[lcut - 1];
        const auto r1 = lcut >= lsz ? INT_MAX : left[lcut];
        const auto l2 = rcut == 0 ? INT_MIN : right[rcut - 1];
        const auto r2 = rcut >= rsz ? INT_MAX : right[rcut];

        if (l1 <= r2 && l2 <= r1) {
            if (sz % 2 == 1) {
                return std::max(l1, l2);
            } else {
                return (std::max(l1, l2) + std::min(r1, r2)) / 2.0;
            }
        } else if (l1 > r2) {
            rbeg = lcut - 1;
        } else {
            lbeg = lcut + 1;
        }
    }

    return 0.0;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": " << solve() << '\n';
    }
}

/*

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 median_of_two_sorted.cpp -o median_of_two_sorted.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address median_of_two_sorted.cpp -o median_of_two_sorted

Run:
median_of_two_sorted.exe < median_of_two_sorted.in

Input:


Output:


*/

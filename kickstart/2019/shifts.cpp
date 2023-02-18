
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

// Shifts
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050e02/000000000018fd5e

int N, H, MX;

constexpr int bits(int x) { return x == 0 ? 0 : 31 - __builtin_clz(x); } // floor(log2(x))

static std::vector<int64_t> solve_inner(const std::vector<int>& data) {
    std::vector<int64_t> ret{};
    std::vector<int64_t> memo(MX);
    for (int m = 1, hi = 0; m < MX; ++m) {
        const int pred = m & (m - 1); // m with lowest bit cleared
        if (pred == 0) {
            memo[m] = data[hi++];
        } else 
            memo[m] = memo[pred] + data[bits(m - pred)];
        if (H <= memo[m])
            ret.push_back(m);
    }

    return ret;
}

static void solve() {
    std::cin >> N >> H;
    assert(N < 32);
    MX = 1 << N;
    std::vector<int> A(N), B(N);
    for (auto& a : A)
        std::cin >> a;
    for (auto& b : B)
        std::cin >> b;

    const auto ashifts = solve_inner(A);
    const auto bshifts = solve_inner(B);

    int ans{};
    for (const auto a : ashifts) {
        const auto to_find = MX - 1 - a;
        const auto start = std::lower_bound(bshifts.begin(), bshifts.end(), to_find);
        ans += std::count_if(start, bshifts.end(), [&](const auto b){
             return (b & to_find) == to_find;
        });
    }

    std::cout << ans;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++17 shifts.cpp -o shifts.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address shifts.cpp -o shifts

Run:
shifts.exe < shifts.in

Input:

2
2 3
1 2
3 3
2 5
2 2
10 30

Output:

Case #1: 3
Case #2: 0

*/


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

// Candy
// https://leetcode.com/problems/candy/description

static void solve_slow() {
    int N;
    std::cin >> N;
    std::vector<int> ratings(N);
    for (auto& r : ratings)
        std::cin >> r;

    int64_t ans{N};
    std::vector<int> candys(N, 1);
    for (int i = 1; i < N; ++i)
        if (ratings[i - 1] < ratings[i]) {
            const auto delta = candys[i - 1] + 1 - candys[i];
            if (delta > 0) {
                candys[i] += delta;
                ans += delta;
            }
        }
    for (int i = N - 2; ~i; --i)
        if (ratings[i] > ratings[i + 1]) {
            const auto delta = candys[i + 1] + 1 - candys[i];
            if (delta > 0) {
                candys[i] += delta;
                ans += delta;
            }
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
        std::cout << "Case #" << g << ": "; solve_slow(); std::cout << '\n';
    }
}

/*

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 candy.cpp -o candy.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address candy.cpp -o candy

Run:
candy.exe < candy.in

Input:

3
3
1 0 2
3
1 2 2
7
3 1 5 1 5 4 2

Output:

Case #1: 5
Case #2: 4
Case #3: 12

*/


#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>

// Maximal decreasing sequence
// Given an integer sequence find the subsequence of maximal sum

const auto rand_in_range = [](const int ubound){ std::random_device seed; std::mt19937 gen{seed()}; std::uniform_int_distribution<int> dist(0, ubound - 1); return dist(gen); };

template <typename C>
static void print(const C& v)
{
    if (v.empty()) return;
    std::cout << v.front();
    const int size = int(v.size());
    for (int i = 1; i < size; ++i)
        std::cout << ' ' << v[i];
    std::cout << '\n';
}

static void solve(const std::vector<int>& input) { // O(NlogA) with O(N) memory
    assert(!input.empty());
    // For every possible ending index of subarray, compute maximal value
    const int size = int(input.size());
    std::vector<int> dp(size);
    dp[0] = input[0];
    for (int i = 1; i < size; ++i)
        dp[i] = input[i] + std::max(0, dp[i - 1]);

    const auto it = std::max_element(dp.begin(), dp.end());
    std::cout << "Maximal subarray has sum " << *it << " and ends at index " << it - dp.begin() << '\n';
}

constexpr int size = 30;
constexpr int range = 10;

int main(int, char**)
{
    std::vector<int> input;
    std::generate_n(std::back_inserter(input), size, []{ return rand_in_range(range) - range / 2; });
    print(input);

    solve(input);
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 maximal_subarray_sum.cpp -o maximal_subarray_sum.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address maximal_subarray_sum.cpp -o maximal_subarray_sum

*/

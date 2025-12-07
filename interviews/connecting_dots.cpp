
#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <vector>

// Connecting Dots
// Given a set of integers on X-axis connect them all minimizing connections length

std::vector<int> input;
constinit const int INF = 1e09;

static int solve() {
    const int sz = int(input.size());
    std::array<int, 2> dp = {0, INF}; // {result for N-2, result for N-1}
    for (int i = 1; i < sz; ++i)
        dp = {dp[1], std::min(dp[0], dp[1]) + input[i] - input[i - 1]};

    return dp[1];
}

int main(int, char**)
{
    input = {1, 2, 3};
    assert(solve() == 2);
    input = {1, 2, 3, 4};
    assert(solve() == 2);
    input = {1, 2, 8, 9};
    assert(solve() == 2);
    input = {1, 2, 3, 8, 9};
    assert(solve() == 3);
    input = {1, 2, 7, 8, 9};
    assert(solve() == 3);
    input = {1, 2, 3, 5, 9};
    assert(solve() == 6);
}

/*

Format:
clang-format -i connecting_dots.cpp

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 connecting_dots.cpp
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address connecting_dots.cpp
clang -Wall -Wextra -Og -ggdb3 -lstdc++ -std=c++20 connecting_dots.cpp

*/

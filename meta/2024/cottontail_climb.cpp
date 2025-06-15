
#include <iostream>
#include <format>
#include <vector>

// Cottontail Climb
// https://www.facebook.com/codingcompetitions/hacker-cup/2024/round-2/problems/A1

std::vector<int64_t> peaks;

static void generate_peaks()
{
    for (int l = 1; l < 10; ++l)
        for (int m = l; m < 10; ++m) {
            int64_t acc{m}, fac{100};
            for (int acl = 1, n = m - 1; acl < l; --n, ++acl, fac *= 100)
                acc = n * fac + acc * 10 + n;
            peaks.push_back(acc);
        }
}

static int64_t solve()
{
    int64_t A, B, M, ans{};
    std::cin >> A >> B >> M;

    for (const auto& p : peaks)
        if (p % M == 0 && A <= p && p <= B)
            ++ans;

    return ans;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    generate_peaks();

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g)
        std::cout << std::format("Case #{}: {}\n", g, solve());
}

/*

Format:
clang-format -i cottontail_climb.cpp

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 cottontail_climb.cpp
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address cottontail_climb.cpp

Run:
a.exe < cottontail_climb.in

Input:

6
121 121 11
0 100 2
0 132 1
121 132 1
121 131 1
22322 22322 1

Output:

Case #1: 1
Case #2: 4
Case #3: 10
Case #4: 1
Case #5: 1
Case #6: 0

*/

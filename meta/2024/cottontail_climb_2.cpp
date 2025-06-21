
#include <iostream>
#include <format>
#include <vector>

// Cottontail Climb - part II
// https://www.facebook.com/codingcompetitions/hacker-cup/2024/round-2/problems/A2
// kudos to: ecnerwala

struct mountain_t
{
    int64_t value;
    int64_t p10;
    int next_left;
    int next_right;
};

std::vector<mountain_t> mountains;

static void generate_mountains()
{
    for (int l = 1; l < 10; ++l)
        mountains.emplace_back(l, 100, l - 1, l - 1);
    for (int i = 0; true; ++i) {
        const auto m = mountains[i]; // copy prevents realloc errors
        for (int l = 1; l <= m.next_left; ++l)
            for (int r = 1; r <= m.next_right; ++r) {
                const auto next = l * m.p10 + m.value * 10 + r;
                if (int64_t(1e18) < next)
                    return;
                mountains.emplace_back(next, m.p10 * 100, l, r);
            }
    }
}

static int64_t solve()
{
    int64_t A, B, M, ans{};
    std::cin >> A >> B >> M;

    for (const auto& p : mountains)
        if (p.value % M == 0 && A <= p.value && p.value <= B)
            ++ans;

    return ans;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    generate_mountains();

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g)
        std::cout << std::format("Case #{}: {}\n", g, solve());
}

/*

Format:
clang-format -i cottontail_climb_2.cpp

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 cottontail_climb_2.cpp
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address cottontail_climb_2.cpp

Run:
a.exe < cottontail_climb_2.in

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
Case #3: 12
Case #4: 3
Case #5: 2
Case #6: 1

*/


#include <iostream>
#include <format>

// Substantial Losses
// https://www.facebook.com/codingcompetitions/hacker-cup/2024/round-1/problems/C

/**
 * Observation 1: we repeat the proces of loosing weight by 1 (W - G) times
 * Observation 2: if F(x) is expected time it takes (in days) to lose 1 unit of weight
 * then F(x) = 0.5 * 1 + 0.5 * (1 + F(x - 1) + F(x))
 * then F(x) = 2x + 1
 */
static const int M = 998'244'353;

static void solve()
{
    int64_t W, G, L;
    std::cin >> W >> G >> L;
    std::cout << ((2 * L + 1) % M) * ((W - G) % M) % M;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << std::format("Case #{}: ", g);
        solve();
        std::cout << '\n';
    }
}

/*

Format:
clang-format -i substantial_losses.cpp

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 substantial_losses.cpp -o substantial_losses.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address substantial_losses.cpp -o substantial_losses

Run:
substantial_losses.exe < substantial_losses.in

Input:

5
201 200 1
185 183 2
250 180 0
77665544332211 11223344556677 0
83716485936440 64528193749358 1938563682

Output:

Case #1: 3
Case #2: 10
Case #3: 70
Case #4: 53884207
Case #5: 306870714

*/

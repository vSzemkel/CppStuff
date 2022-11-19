
#include <array>
#include <iostream>

// Magical Well Of Lilies
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb1b6/0000000000c47e79

constexpr const int INF = 1e09;
constexpr const int MAX = 1e05;

int L, ans;
std::array<int, MAX + 1> dp;

/**
 * @brief For each i compute one coin operation backward
 *        and sequence of two coins operation forward
 */
static void precompute() {
    dp.fill(INF);
    dp[0] = 0;
    dp[1] = 1;
    for (int i = 2; i <= MAX; ++i) {
        dp[i] = std::min(dp[i], dp[i - 1] + 1);
        for (int j = 2 * i, c = 6; j <= MAX; j += i, c += 2)
            dp[j] = std::min(dp[j], dp[i] + c);
    }
}

static void solve() {
    std::cin >> L;
    std::cout << dp[L];
}

static void inner_solve_set1(int acc, int mem, int cost) { // O(3^N)
    if (acc >= L || cost > ans) {
        if (acc == L && cost < ans)
            ans = cost;
        return;
    }

    inner_solve_set1(acc + 1, mem, cost + 1);
    if (0 < mem)
        inner_solve_set1(acc + mem, mem, cost + 2);
    if (mem < acc)
        inner_solve_set1(acc, acc, cost + 4);
}

static void solve_set1() {
    std::cin >> L;

    ans = 1e09;
    inner_solve_set1(0, 0, 0);
    std::cout << ans;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;

    precompute();

    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 magical_well_of_lilies.cpp -o magical_well_of_lilies.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address magical_well_of_lilies.cpp -o magical_well_of_lilies

Run:
magical_well_of_lilies.exe < magical_well_of_lilies.in

Input:

3
17
120
1974

Output:

Case #1: 15
Case #2: 28
Case #3: 50

*/


#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

// Unlock The Padlock
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008caa74/0000000000acef55

/**
 * @brief We keep track of the side with current value zero
 */
static void solve() {
    int N, D;
    std::cin >> N >> D;
    std::vector<int> disks(N);
    for (auto& d : disks)
        std::cin >> d;

    const auto distance = [&](const int a, const int b) {
        const int c = std::abs(a - b);
        return std::min(c, D - c);
    };

    std::vector<std::vector<std::vector<int64_t>>> dp(N, std::vector<std::vector<int64_t>>(N, std::vector<int64_t>(2, -1LL)));
    std::function<int64_t(int, int, int)> solve_inner = [&](const int lb, const int rb, const int dir) {
        if (~dp[lb][rb][dir])
            return dp[lb][rb][dir];

        const int cur0 = disks[dir == 0 ? lb - 1 : rb + 1];
        if (lb == rb)
            return dp[lb][rb][dir] = distance(cur0, disks[lb]);

        const int64_t from_left = distance(cur0, disks[lb]) + solve_inner(lb + 1, rb, 0);
        const int64_t from_right = solve_inner(lb, rb - 1, 1) + distance(cur0, disks[rb]);
        return dp[lb][rb][dir] = std::min(from_left, from_right);
    };

    if (N == 1)
        std::cout << distance(disks.front(), 0);
    else
        std::cout << std::min(distance(disks.front(), 0) + solve_inner(1, N - 1, 0), 
                              distance(disks.back(),  0)  + solve_inner(0, N - 2, 1));
}

/**
 * @brief We keep track of current value of hypothetical disk with value 0 in the middle of the range
 */
static void solve_set2() {
    int N, D;
    std::cin >> N >> D;
    std::vector<int> disks(N);
    for (auto& d : disks)
        std::cin >> d;

    const auto distance = [&](const int a, const int b) {
        const int c = std::abs(a - b);
        return std::min(c, D - c);
    };

    std::vector<std::vector<std::vector<int64_t>>> dp(N, std::vector<std::vector<int64_t>>(N, std::vector<int64_t>(D, -1LL)));
    std::function<int64_t(int, int, int)> solve_inner = [&](const int lb, const int rb, const int cur0) {
        if (~dp[lb][rb][cur0])
            return dp[lb][rb][cur0];

        const int lval = (disks[lb] + cur0 + D) % D;
        if (lb == rb)
            return dp[lb][rb][cur0] = distance(lval, 0);

        const int64_t from_left = distance(lval, 0) + solve_inner(lb + 1, rb, (cur0 - lval + D) % D);
        const int rval = (disks[rb] + cur0 + D) % D;
        const int64_t from_right = solve_inner(lb, rb - 1, (cur0 - rval + D) % D) + distance(rval, 0);

        return dp[lb][rb][cur0] = std::min(from_left, from_right);
    };

    std::cout << solve_inner(0, N - 1, 0);
}

/**
 * @brief Greegy solution works only with D == 2
 */
static void solve_set1() {
    int N, D;
    std::cin >> N >> D;
    std::vector<int> disks(N);
    for (auto& d : disks)
        std::cin >> d;

    const auto distance = [&](const int a, const int b) {
        const int c = std::abs(a - b);
        return std::min(c, D - c);
    };

    int cur0{0}; // value of 0 after applying all operations so far
    int ans{0}, lb{0}, rb{N - 1};
    while (true) {
        while (lb < N && disks[lb] == cur0) ++lb;
        while (rb >= 0 && disks[rb] == cur0) --rb;
        if (rb < lb) break;

        int cost, target;
        const int lval = (disks[lb] + cur0 + D) % D;
        const int rval = (disks[rb] + cur0 + D) % D;
        if (distance(lval, 0) < distance(rval, 0)) {
            cost = distance(lval, 0);
            target = lval;
        } else {
            cost = distance(rval, 0);
            target = rval;
        }

        ans += cost;
        cur0 = (cur0 - target + D) % D;
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
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 unlock_the_padlock.cpp -o unlock_the_padlock.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address unlock_the_padlock.cpp -o unlock_the_padlock

Run:
unlock_the_padlock.exe < unlock_the_padlock.in

Input:

2
6 10
1 1 2 2 3 3
6 10
1 1 9 9 1 1

Output:

Case #1: 3
Case #2: 3

*/

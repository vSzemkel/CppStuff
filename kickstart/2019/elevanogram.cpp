
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

// Elevanogram
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050edd/00000000001a286d

static void solve() {
    // 11 is relative prime to every digit so having 10 equal items
    // we can arrange them to have any possible reminder
    std::vector<int> cnt(10), psum(10);
    for (int i = 1; i < 10; ++i) {
        int c; std::cin >> c;
        cnt[i] = std::min(c, 10 + (c & 1));
        psum[i] = psum[i - 1] + cnt[i];
    }


    const int size = psum.back();
    // dp[i][j] means is it possible to have a rest of j mod 11 having i positive components
    std::vector<std::vector<bool>> dp(size + 1, std::vector<bool>(11));
    dp[0][0] = true;
    for (int digit = 1; digit < 10; ++digit) {
        const int count = cnt[digit], total = psum[digit];
        std::vector<std::vector<bool>> ndp(size + 1, std::vector<bool>(11));
        for (int pick = 0; pick <= count; ++pick)
            for (int pluses = 0; pluses + pick <= total; ++pluses)
                for (int rem = 0; rem < 11; ++rem) {
                    const int nrem = (rem + digit * pick + (11 - digit) * (count - pick)) % 11;
                    if (dp[pluses][rem])
                        ndp[pluses + pick][nrem] = true;
                }

        dp = std::move(ndp);
    }

    std::cout << (dp[size / 2][0] ? "YES" : "NO");
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
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++17 elevanogram.cpp -o elevanogram.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address elevanogram.cpp -o elevanogram

Run:
elevanogram.exe < elevanogram.in

Input:

6
0 0 2 0 0 1 0 0 0
0 0 0 0 0 0 0 0 12
0 0 0 0 2 0 1 1 0
3 1 1 1 0 0 0 0 0
3 0 0 0 0 0 3 0 2
0 0 0 0 0 0 0 1 0

Output:

Case #1: YES
Case #2: YES
Case #3: NO
Case #4: YES
Case #5: YES
Case #6: NO

*/

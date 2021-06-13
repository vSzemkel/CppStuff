
#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

// Alien Piano
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ff08/0000000000387174


static void solve() {
    int N, prev; std::cin >> N >> prev;
    std::vector<int> pitches;
    for (int i = 1; i < N; ++i) {
        int p; std::cin >> p;
        if (p < prev)
            pitches.push_back(-1);
        else if (prev < p)
            pitches.push_back(1);
        prev = p;
    }

    int ret{0}, score{0};
    for (const auto p : pitches) {
        if (p * score < 0) // resetting event
            score = 0;

        score += p;

        if (score < -3 || score > 3) {
            score = 0;
            ++ret;
        }
    }

    std::cout << ret;
}

static void solve_dp() {
    int N, prev; std::cin >> N >> prev;
    std::vector<int> pitches;
    for (int i = 1; i < N; ++i) {
        int p; std::cin >> p;
        if (p < prev)
            pitches.push_back(-1);
        else if (prev < p)
            pitches.push_back(1);
        prev = p;
    }

    std::array<int, 4> dp; // cost of processing with particular last pitch
    dp.fill(0);
    for (const auto p : pitches) {
        std::array<int, 4> step_dp;
        step_dp.fill(N);
        for (int prev = 0; prev < 4; ++prev)
            for (int next = 0; next < 4; ++next) {
                int penalty{0};
                if (prev <= next && p < 0) ++penalty;
                if (prev >= next && p > 0) ++penalty;
                step_dp[next] = std::min(step_dp[next], dp[prev] + penalty);
            }
        dp = step_dp;
    }

    std::cout << *std::min_element(dp.begin(), dp.end());
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 alien_piano.cpp -o alien_piano.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address alien_piano.cpp -o alien_piano

Run:
alien_piano.exe < alien_piano.in

Input:

2
5
1 5 100 500 1
8
2 3 4 5 6 7 8 9

Output:

Case #1: 0
Case #2: 1

*/

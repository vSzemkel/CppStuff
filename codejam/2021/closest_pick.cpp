
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>

// Closest Pick
// https://codingcompetitions.withgoogle.com/codejam/round/00000000004362d7/00000000007c0f00


static void solve() { // DO NOT EVER SCAN 10**9 SIZE ARRAY
    int N, K; std::cin >> N >> K;
    std::vector<int> raffle(N);
    for (int i = 0; i < N; ++i)
        std::cin >> raffle[i];
    std::sort(raffle.begin(), raffle.end());

    int left{0}, right{0}; 
    std::vector<std::pair<int, bool>> gaps; // {size, extreme}
    if (raffle[0] > 1) {
        left = raffle[0] - 1;
        gaps.emplace_back(left, true);
    }
    if (raffle.back() < K) {
        right = K - raffle.back();
        gaps.emplace_back(right, true);
    }
    for (int i = 1; i < N; ++i) {
        const int gap = raffle[i] - raffle[i - 1] - 1;
        if (gap > 0)
            gaps.emplace_back(gap, false);
    }

    if (gaps.empty()) {
        std::cout << '0';
        return;
    }

    std::make_heap(gaps.begin(), gaps.end());
    int winning = gaps[0].first;
    if (gaps.size() > 1) {
        if (!gaps[0].second)
            winning = (winning + 1) / 2;
        int reserve = gaps[0].first - winning;
        std::pop_heap(gaps.begin(), gaps.end());
        const int second_gap = gaps[0].second ? gaps[0].first : (gaps[0].first + 1) / 2;
        if (reserve > 0)
            reserve = std::max(reserve, std::max(left, right));
        winning += std::max(reserve, second_gap);
        winning = std::max(left + right, winning);
    }

    std::cout << double(winning) / K;
}

static void solve_nqiiii() { // inspired by nqiiii, simpler but slower
    int N, K; std::cin >> N >> K;
    std::vector<int> raffle(N);
    for (int i = 0; i < N; ++i)
        std::cin >> raffle[i];
    std::sort(raffle.begin(), raffle.end());

    std::vector<int> gaps; // how many winning slots in gap after single optimal move
    if (raffle[0] > 1)
        gaps.push_back(raffle[0] - 1);
    if (raffle.back() < K)
        gaps.push_back(K - raffle.back());
    for (int i = 1; i < N; ++i) {
        const int gap = (raffle[i] - raffle[i - 1]) / 2;
        if (gap > 0)
            gaps.push_back(gap);
    }

    if (gaps.empty()) {
        std::cout << '0';
        return;
    }

    std::make_heap(gaps.begin(), gaps.end());
    int winning = gaps[0];
    if (gaps.size() > 1) {
        std::pop_heap(gaps.begin(), gaps.end());
        winning += gaps[0];
    }
    for (int i = 1; i < N; ++i) // what if two moves in one gap
        winning = std::max(winning, raffle[i] - raffle[i - 1] - 1);

    std::cout << double(winning) / K;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": " << std::fixed << std::setprecision(9); solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 closest_pick.cpp -o closest_pick.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address closest_pick.cpp -o closest_pick.o

Run:
closest_pick.exe < closest_pick.in

Input:

6
3 10
1 3 7
4 10
4 1 7 3
4 3
1 2 3 2
4 4
1 2 4 2
2 10
1 10
17 159554872
14547803 59698553 110344370 43954042 85045951 109817753 79548304 155861121 132562545 110177597 83036767 73864910 22090220 102531069 13450582 74221172 132525777

Output:

Case #1: 0.500000000
Case #2: 0.400000000
Case #3: 0
Case #4: 0.250000000
Case #5: 0.800000000
Case #6: 0.157311831

*/


#include <iostream>
#include <numeric>
#include <vector>

// Candy
// https://leetcode.com/problems/candy/description

constexpr int INF = 1e09;

static void solve() {
    int N;
    std::cin >> N;
    if (N == 1) {
        std::cout << 1;
        return;
    }

    std::vector<int> ratings(N);
    for (auto& r : ratings)
        std::cin >> r;

    int64_t candys{N};
    for (int i = 1; i < N; ) {
        if (ratings[i - 1] == ratings[i]) {
            ++i;
            continue;
        }

        int peek{0};
        while (i < N && ratings[i - 1] < ratings[i]) {
            ++peek;
            candys += peek;
            ++i;
        }

        int valley{0};
        while (i < N && ratings[i - 1] > ratings[i]) {
            ++valley;
            candys += valley;
            ++i;
        }

        // added peak twice, once as peak, once as valley, correct
        candys -= std::min(peek, valley);
    }

    std::cout << candys;
}

static void solve_fast () {
    int N;
    std::cin >> N;
    if (N == 1) {
        std::cout << 1;
        return;
    }

    std::vector<int> ratings(N);
    for (auto& r : ratings)
        std::cin >> r;

    std::vector<int> candys(N, INF);

    // find valleys
    if (ratings[0] <= ratings[1])
        candys[0] = 1;
    for (int i = 1; i < N - 1; ++i)
        if (ratings[i] <= ratings[i - 1] && ratings[i] <= ratings[i + 1])
            candys[i] = 1;
    if (ratings[N - 1] <= ratings[N - 2])
        candys[N - 1] = 1;
    // propagate
    for (int i = 1; i < N; ++i)
        if (ratings[i - 1] < ratings[i]) {
            if (candys[i] == INF)
                candys[i] = candys[i - 1] + 1;
            else
                candys[i] = std::max(candys[i], candys[i - 1] + 1);
        }
    for (int i = N - 2; ~i; --i)
        if (ratings[i] > ratings[i + 1]) {
            if (candys[i] == INF)
                candys[i] = candys[i + 1] + 1;
            else
                candys[i] = std::max(candys[i], candys[i + 1] + 1);
        }

    std::cout << std::accumulate(candys.begin(), candys.end(), 0LL);
}

static void solve_memo() {
    int N;
    std::cin >> N;
    std::vector<int> ratings(N);
    for (auto& r : ratings)
        std::cin >> r;

    std::vector<int> candys(N, 1);
    for (int i = 1; i < N; ++i)
        if (ratings[i - 1] < ratings[i]) {
            const auto delta = candys[i - 1] + 1 - candys[i];
            if (delta > 0)
                candys[i] += delta;
        }
    for (int i = N - 2; ~i; --i)
        if (ratings[i] > ratings[i + 1]) {
            const auto delta = candys[i + 1] + 1 - candys[i];
            if (delta > 0)
                candys[i] += delta;
        }

    std::cout << std::accumulate(candys.begin(), candys.end(), 0LL);
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
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 candy.cpp -o candy.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address candy.cpp -o candy

Run:
candy.exe < candy.in

Input:

3
3
1 0 2
3
1 2 2
7
3 1 5 1 5 4 2

Output:

Case #1: 5
Case #2: 4
Case #3: 12

*/


#include <iostream>
#include <vector>

// Matrygons
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000435915/00000000007dbf06

constexpr int MAX = 1'000'001;
std::vector<int> dp(MAX, 1);
std::vector<int> answers(MAX, 1);

static void init() {
    for (int n = 2; n < MAX; ++n) { // total edges
        const int candidate = dp[n] + 1;
        for (int m = 2, e = 2 * (n + 1); e < MAX; ++m, e += (n + 1)) { // m == inner, e == n * m + m
            if (dp[e] < candidate)
                dp[e] = candidate;
            if (m > 2 && answers[e] < candidate)
                answers[e] = candidate;
        }
    }
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    init();
    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        int N; std::cin >> N;
        std::cout << "Case #" << g << ": " << answers[N] << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 matrygons.cpp -o matrygons.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address matrygons.cpp -o matrygons

Run:
matrygons.exe < matrygons.in

Input:

3
33
15
41

Output:

Case #1: 3
Case #2: 2
Case #3: 1

*/

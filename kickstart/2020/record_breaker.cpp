
#include <iostream>
#include <vector>

// Record breaker
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ff08/0000000000387171



static int solve() {
    int N; std::cin >> N;
    std::vector<int> scores(N);
    for (auto& s : scores)
        std::cin >> s;

    if (N < 2) return 1;
    int max = scores[0];
    int ret = (max > scores[1]) ? 1 : 0;
    for (int i = 1; i < N - 1; ++i) {
        const auto& s = scores[i];
        if (max < s && s > scores[i + 1])
            ++ret;
        max = std::max(max, s);
    }
    if (max < scores.back())
        ++ret;

    return ret;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": " << solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 record_breaker.cpp -o record_breaker.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address record_breaker.cpp -o record_breaker

Run:
record_breaker.exe < record_breaker.in

Input:

4
8
1 2 0 7 2 0 2 0
6
4 8 15 16 23 42
9
3 1 4 1 5 9 2 6 5
6
9 9 9 9 9 9

Output:

Case #1: 2
Case #2: 1
Case #3: 3
Case #4: 0

*/

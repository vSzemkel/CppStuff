
#include <iostream>
#include <numeric>
#include <string>

// K-Goodness String
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000436140/000000000068cca3


static void solve() {
    int64_t N, K; std::cin >> N >> K;
    std::string text; std::cin >> text;

    int64_t score{0};
    for (int i = 0; i < N / 2; ++i)
        score += (text[i] != text[N - i - 1]) ? 1 : 0;

    std::cout << std::abs(K - score);
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 kgoodness_string.cpp -o kgoodness_string.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 kgoodness_string.cpp -o kgoodness_string.o

Run:
kgoodness_string.exe < kgoodness_string.in

Input:

2
5 1
ABCAA
4 2
ABAA

Output:

Case #1: 0
Case #2: 1

*/

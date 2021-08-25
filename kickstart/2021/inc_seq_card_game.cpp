
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

// Incremental sequence game card
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000043585c/000000000085a709#problem


double factorial(const int n) {
    return std::tgamma(n + 1);
}

static void naive() {
    int64_t N; std::cin >> N;
    std::vector<int64_t> cards(N);
    std::iota(cards.begin(), cards.end(), 0);

    int64_t ret{0};
    do {
        int64_t cur{1}, last = cards.front();
        for (int i = 1; i < N; ++i)
            if (cards[i] > last) {
                last = cards[i];
                ++cur;
            }
        ret += cur;
    } while (std::next_permutation(cards.begin(), cards.end()));

    std::cout << ret / factorial(N);
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": " << std::fixed << std::setprecision(9); naive(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 inc_seq_card_game.cpp -o inc_seq_card_game.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address inc_seq_card_game.cpp -o inc_seq_card_game

Run:
inc_seq_card_game.exe < inc_seq_card_game.in

Input:

1
10

Output:

Case #1: 2.928968254

*/

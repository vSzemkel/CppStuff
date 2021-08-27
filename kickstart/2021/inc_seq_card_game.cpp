
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

// solution is recursive. For N+1 positioning highest card with number N+1
// can be done in N+1 ways, and every choice discards cards with lower numbers

int64_t solved{3};
std::vector<long double> score_partial_sum = {0.0, 1.0, 2.5};
constexpr int64_t MAXEXACT = 1000000LL;

static void solve() {
    int64_t N; std::cin >> N;

    for (int64_t n = solved; n < N; ++n) {
        const auto last = score_partial_sum.back();
        score_partial_sum.push_back(last + (last / n) + 1);
    }
    solved = std::max(solved, N);

    std::cout << score_partial_sum[N - 1] / N + 1;
}

// observ that EX[N + 1] == EX[N] + 1 / (N + 1)
// EX[N + 1] = S[N] / (N + 1) + 1 = (S[N - 1] + EX[N]) / (N + 1) + 1
// (S[N - 1] + S[N - 1] / N + 1) / (N + 1) + 1
// ((N + 1) * S[N - 1] / N + 1) / N + 1) + 1
// S[N - 1] / N + (1 / (N + 1)) + 1
// EX[N] + 1 / (N + 1)
static void solve_math() {
    int64_t N; std::cin >> N;

    const auto bound = std::min(N, MAXEXACT);
    for (int64_t n = solved; n < bound; ++n) {
        const auto last = score_partial_sum.back();
        score_partial_sum.push_back(last + (last / n) + 1);
    }
    solved = std::max(solved, bound);

    if (N <= bound)
        std::cout << score_partial_sum[N - 1] / N + 1;
    else
        std::cout << score_partial_sum[MAXEXACT - 1] / MAXEXACT + 1 + std::log(N) - std::log(MAXEXACT + 1);
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
        std::cout << "Case #" << g << ": " << std::fixed << std::setprecision(9); solve_math(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 inc_seq_card_game.cpp -o inc_seq_card_game.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address inc_seq_card_game.cpp -o inc_seq_card_game

Run:
inc_seq_card_game.exe < inc_seq_card_game.in

Input:

3
10
1000000
100000000000000

Output:

Case #1: 2.928968254
Case #2: 14.392726723
Case #3: 32.813406467

*/

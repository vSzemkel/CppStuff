
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

// Join The Ranks
// https://codingcompetitions.withgoogle.com/codejam/round/000000000019fef2/00000000002d5b64

void solve() {
    int R, S; std::cin >> R >> S;
    const int size = R * S;
    std::vector<int> cards;
    std::vector<std::pair<int, int>> moves;
    for (int i = 1; i <= S; ++i)
        for (int j = 1; j <= R; ++j)
            cards.emplace_back(j);

    /* Naive, too costly
    for (int b = 0; b < R - 1; ++b) {
        const auto bottom = cards.end() - b * S;
        for (int i = 0; i < S - 1; ++i) {
            const auto end = bottom - i - 1;
            const auto mid = end - R + b + 1;
            std::rotate(cards.begin(), mid, end);
            moves.emplace_back(mid - cards.begin(), end - mid);
        }
    }*/
    
    /* Optimal cost, but illegal
    for (int b = 0; 2 * b < R - 1; ++b) {
        const auto beg = cards.begin() + b * S;
        const auto bottom = cards.end() - b * S;
        for (int i = 0; i < S - i; ++i) {
            const auto end = bottom - i - 1;
            const auto mid = end - R + 2 * b + 1;
            std::rotate(beg + i + 1, mid, end);
        }
    }*/

    // General idea: one rotate decreases frontier count by 2
    // aaabbbcccdddaaabbb   5 frontiers
    // cccdddaaaaaabbbbbb   3 frontiers

    while (true) {
        int mid{0};
        const int topval = cards[0];
        while (cards[mid] == topval) ++mid;
        const int midval = cards[mid];
        while (mid < size && cards[mid] == midval) ++mid;
        int end = size - 2;
        while (!(cards[end] == topval && cards[end + 1] == midval)) --end;
        if (mid >= end + 1) break;
        std::rotate(cards.begin(), cards.begin() + mid, cards.begin() + end + 1);
        moves.emplace_back(mid, end + 1 - mid);
    }

    int bad{0};
    while (cards[bad] != 1) ++bad;
    if (bad > 0) {
        std::rotate(cards.begin(), cards.begin() + bad, cards.end());
        moves.emplace_back(bad, size - bad);
    }

    std::cout << moves.size() << "\n";
    for (const auto& rs : moves)
        std::cout << rs.first << " " << rs.second << "\n";
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve();
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 join_the_ranks.cpp -o join_the_ranks.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 join_the_ranks.cpp -o join_the_ranks.o

Run:
join_the_ranks.exe < join_the_ranks.in

Input:

3
2 2
3 2
2 3

Output:

Case #1: 1
2 1
Case #2: 2
2 2
1 5
Case #3: 2
2 3
2 2

*/

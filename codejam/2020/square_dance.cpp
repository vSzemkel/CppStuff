
#include <algorithm>
#include <iostream>
#include <vector>

// Square Dance
// https://codingcompetitions.withgoogle.com/codejam/round/000000000019fd74/00000000002b1355

struct dancer_t
{
    int skill;
    int tr; // top active row
    int br; // bottom
    int lc; // left active column
    int rc; // right
    bool well_dancing;
};

int g_rows, g_cols;
std::vector<dancer_t> g_board;

void quallify(dancer_t& d)
{
    int sum{0};
    if (d.tr >= 0) sum += d.skill - g_board[d.tr].skill;
    if (d.br >= 0) sum += d.skill - g_board[d.br].skill;
    if (d.rc >= 0) sum += d.skill - g_board[d.rc].skill;
    if (d.lc >= 0) sum += d.skill - g_board[d.lc].skill;

    d.well_dancing = sum >= 0;
}

int64_t solve() {
    std::cin >> g_rows >> g_cols;
    const int g_size = g_rows * g_cols;
    g_board.resize(g_size);
    // read input
    for (int d = 0; d < g_size; ++d)
        std::cin >> g_board[d].skill;
    // initialize
    int64_t ret{0}, survivors_skills{0};
    std::vector<int> still_dancing(g_size);
    for (int i = 0; i < g_size; ++i) {
        auto& d = g_board[i];
        d.lc = i % g_cols > 0 ? i - 1 : -1;
        d.rc = i % g_cols < g_cols - 1 ? i + 1 : -1;
        d.tr = i / g_cols > 0 ? i - g_cols : -1;
        d.br = i / g_cols < g_rows - 1 ? i + g_cols : -1;
        quallify(d);
        still_dancing[i] = i;
        survivors_skills += d.skill;
    }
    // play rounds
    while (true) {
        ret += survivors_skills;
        const auto victims = std::partition(still_dancing.begin(), still_dancing.end(), [](const int pos){ return g_board[pos].well_dancing; });
        if (victims == still_dancing.end()) break;

        for (auto v = victims; v != still_dancing.end(); ++v) {
            const int pos = *v;
            const auto& d = g_board[pos];
            survivors_skills -= d.skill;
            if (d.lc >= 0) {
                auto& w = g_board[d.lc];
                w.rc = d.rc;
                quallify(w);
            }
            if (d.rc >= 0) {
                auto& e = g_board[d.rc];
                e.lc = d.lc;
                quallify(e);
            }
            if (d.tr >= 0) {
                auto& n = g_board[d.tr];
                n.br = d.br;
                quallify(n);
            }
            if (d.br >= 0) {
                auto& s = g_board[d.br];
                s.tr = d.tr;
                quallify(s);
            }
        }

        still_dancing.erase(victims, still_dancing.end());
    }

    return ret;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases; std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g)
        std::cout << "Case #" << g << ": " << solve() << "\n";
}

/*
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 square_dance.cpp -o square_dance.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 square_dance.cpp -o square_dance.o
square_dance.exe < square_dance.in

Input:

4
1 1
15
3 3
1 1 1
1 2 1
1 1 1
1 3
3 1 2
1 3
1 2 3

Output:

Case #1: 15
Case #2: 16
Case #3: 14
Case #4: 14

*/

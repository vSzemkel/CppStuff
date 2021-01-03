
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

// Square Dance
// https://codingcompetitions.withgoogle.com/codejam/round/000000000019fd74/00000000002b1355

using coord_t = std::pair<int, int>;

struct dancer_t
{
    int skill;
    int N, S, E, W;
    int dist_N, dist_S, dist_E, dist_W;
    bool well_dancing;
};

int g_rows, g_cols;
std::vector<std::vector<dancer_t>> g_board;

void quallify(dancer_t& d)
{
    int sum{0}, cnt{0};
    if (d.N > 0) { sum += d.N; ++cnt; }
    if (d.S > 0) { sum += d.S; ++cnt; }
    if (d.E > 0) { sum += d.E; ++cnt; }
    if (d.W > 0) { sum += d.W; ++cnt; }

    d.well_dancing = cnt == 0 || d.skill >= (double)sum / cnt;
}

int64_t solve() {
    std::cin >> g_rows >> g_cols;
    g_board.assign(g_rows, std::vector<dancer_t>(g_cols));
    // read input
    for (int r = 0; r < g_rows; ++r)
        for (int c = 0; c < g_cols; ++c)
            std::cin >> g_board[r][c].skill;
    // initialize
    std::vector<coord_t> still_dancing;
    int64_t ret{0}, survivors_skills{0};
    for (int r = 0; r < g_rows; ++r)
        for (int c = 0; c < g_cols; ++c) {
            auto& d = g_board[r][c];
            if (c > 0) {
                d.W = g_board[r][c - 1].skill;
                d.dist_W = 1;
            }
            if (c < g_cols - 1) {
                d.E = g_board[r][c + 1].skill;
                d.dist_E = 1;
            }
            if (r > 0) {
                d.N = g_board[r - 1][c].skill;
                d.dist_N = 1;
            }
            if (r < g_rows - 1) {
                d.S = g_board[r + 1][c].skill;
                d.dist_S = 1;
            }
            quallify(d);
            survivors_skills += d.skill;
            still_dancing.emplace_back(r, c);
        }
    // play rounds
    while (true) {
        ret += survivors_skills;
        const auto victims = std::partition(still_dancing.begin(), still_dancing.end(), [](const coord_t& d){ return g_board[d.first][d.second].well_dancing; });
        if (victims == still_dancing.end()) break;

        for (auto v = victims; v != still_dancing.end(); ++v) {
            const auto [r, c] = *v;
            const auto& d = g_board[r][c];
            survivors_skills -= d.skill;
            if (c > 0 && d.dist_W > 0) {
                auto& w = g_board[r][c - d.dist_W];
                if (d.dist_E > 0) {
                    w.E = d.E;
                    w.dist_E += d.dist_E;
                } else
                    w.dist_E = w.E = 0;
                quallify(w);
            }
            if (c < g_cols - 1 && d.dist_E > 0) {
                auto& e = g_board[r][c + d.dist_E];
                if (d.dist_W > 0) {
                    e.W = d.W;
                    e.dist_W += d.dist_W;
                } else
                    e.dist_W = e.W = 0;
                quallify(e);
            }
            if (r > 0 && d.dist_N > 0) {
                auto& n = g_board[r - d.dist_N][c];
                if (d.dist_S > 0) {
                    n.S = d.S;
                    n.dist_S += d.dist_S;
                } else
                    n.dist_S = n.S = 0;
                quallify(n);
            }
            if (r < g_rows - 1 && d.dist_S > 0) {
                auto& s = g_board[r + d.dist_S][c];
                if (d.dist_S > 0) {
                    s.N = d.N;
                    s.dist_N += d.dist_N;
                } else
                    s.dist_N = s.N = 0;
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

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": " << solve() << "\n";
    }
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

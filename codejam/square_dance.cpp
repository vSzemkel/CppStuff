
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
};

int g_rows, g_cols;
std::vector<std::vector<dancer_t>> g_board;

bool quallify(const coord_t& coord)
{
    int sum{0}, cnt{0};
    const auto& d = g_board[coord.first][coord.second];
    if (d.N > 0) {
        sum += d.N;
        ++cnt;
    }
    if (d.S > 0) {
        sum += d.S;
        ++cnt;
    }
    if (d.E > 0) {
        sum += d.E;
        ++cnt;
    }
    if (d.W > 0) {
        sum += d.W;
        ++cnt;
    }

    return cnt == 0 || d.skill >= (double)sum / cnt;
}

int64_t solve() {
    std::cin >> g_rows >> g_cols;
    g_board.assign(g_rows, std::vector<dancer_t>(g_cols));
    // read input
    for (int r = 0; r < g_rows; ++r)
        for (int c = 0; c < g_cols; ++c)
            std::cin >> g_board[r][c].skill;
    // initialize
    std::vector<coord_t> dancing;
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
            survivors_skills += d.skill;
            dancing.emplace_back(r, c);
        }
    // play rounds
    while (true) {
        ret += survivors_skills;
        const auto victims = std::partition(dancing.begin(), dancing.end(), [](const coord_t& d){ return quallify(d); });
        if (victims == dancing.end()) break;

        for (auto v = victims; v != dancing.end(); ++v) {
            const auto [r, c] = *v;
            const auto& d = g_board[r][c];
            survivors_skills -= d.skill;
            if (c > 0 && d.dist_W > 0) {
                auto& wd = g_board[r][c - d.dist_W];
                if (d.dist_E > 0) {
                    wd.E = d.E;
                    wd.dist_E += d.dist_E;
                } else
                    wd.dist_E = wd.E = 0;
            }
            if (c < g_cols - 1 && d.dist_E > 0) {
                auto& ed = g_board[r][c + d.dist_E];
                if (d.dist_W > 0) {
                    ed.W = d.W;
                    ed.dist_W += d.dist_W;
                } else
                    ed.dist_W = ed.W = 0;
            }
            if (r > 0 && d.dist_N > 0) {
                auto& nd = g_board[r - d.dist_N][c];
                if (d.dist_S > 0) {
                    nd.S = d.S;
                    nd.dist_S += d.dist_S;
                } else
                    nd.dist_S = nd.S = 0;
            }
            if (r < g_rows - 1 && d.dist_S > 0) {
                auto& sd = g_board[r + d.dist_S][c];
                if (d.dist_S > 0) {
                    sd.N = d.N;
                    sd.dist_N += d.dist_N;
                } else
                    sd.dist_N = sd.N = 0;
            }
        }

        dancing.erase(victims, dancing.end());
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


Output:


*/

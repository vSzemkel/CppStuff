
#include <algorithm>
#include <iostream>
#include <numeric>
#include <unordered_set>
#include <vector>

// Robot Programming Strategy
// https://codingcompetitions.withgoogle.com/codejam/round/00000000000516b9/0000000000134c90

constexpr int g_maxlen = 500;
std::vector<std::vector<std::string>> g_cases;

constexpr char trump(const char move)
{
    if (move == 'P') return 'S';
    if (move == 'S') return 'R';
    if (move == 'R') return 'P';
    return move;
}

// for those who are still stuck on C++14
/*int lcm(int l1, int l2)
{
    if (l1 > l2)
        std::swap(l1, l2);

    int lcm = l2;
    while (true) {
        if (lcm % l1 == 0)
            return lcm;
        lcm += l2;
    }
}*/

int main(int argc, char* argv[])
{
    // parse console input
    int no_of_cases, no_of_players;
    std::cin >> no_of_cases;
    g_cases.resize(no_of_cases);
    for (auto& game : g_cases) {
        std::cin >> no_of_players;
        game.resize(no_of_players);
        while (--no_of_players >= 0)
            std::cin >> game[no_of_players];
    }

    // padding not needed, but I like std::accumulate application with std::lcm
    /*for (auto& game : g_cases) {
        const int dest = std::min(g_maxlen, std::accumulate(game.begin(), game.end(), 1, [](const int i, const auto& s) { return std::lcm(i, s.size()); }));
        for (auto& program : game) {
            const int short_size = program.size();
            for (int i = 0; program.size() < dest; ++i)
                program.push_back(program[i % short_size]);
        }
    }*/

    // solution
    for (int c = 0; c < no_of_cases; ++c) {
        std::string solution;
        std::unordered_set<char> moves;
        auto& game = g_cases[c];
        for (int i = 0; i < g_maxlen; ++i) {
            char next_move;
            moves.clear();
            for (auto& program : game) {
                const char opponent = program[i % program.size()];
                moves.insert(opponent);
                int distinct_moves = moves.size();
                if (distinct_moves == 3) // impossible
                    goto solved;
                next_move = trump(opponent);
                if (distinct_moves == 2) { // tie move
                    if (moves.count(next_move) == 0)
                        next_move = opponent;
                }
            }

            solution.push_back(next_move);

            // eliminate defeated
            const auto it = std::remove_if(game.begin(), game.end(), [i, next_move](const auto& s) { return trump(s[i % s.size()]) == next_move; });
            game.erase(it, game.end());
            if (game.empty()) break;
        }

    solved:
        if (game.empty() && solution.size() <= g_maxlen)
            std::cout << "Case #" << (c + 1) << ": " << solution << "\n";
        else
            std::cout << "Case #" << (c + 1) << ": IMPOSSIBLE\n";
    }
}

/* clang++.exe -Wall -g -O0 -std=c++17 robot_strategy.cpp -o robot_strategy.exe
robot_strategy.exe < robot_strategy.in

Input:

6
6
PRPPPP
PR
PS
RS
RS
RR
2
RS
PR
1
RS
3
R
P
S
7
RS
RS
RS
RS
RS
RS
RS
2
RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
PPPPPPPPPPPPPPPPPPPPPPPPPPPP

Output:

Case #1: PRS
Case #2: PP
Case #3: P
Case #4: IMPOSSIBLE
Case #5: P
Case #6: PS

*/
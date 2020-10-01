
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>

// Robot Programming Strategy
// https://codingcompetitions.withgoogle.com/codejam/round/00000000000516b9/0000000000134c90

std::vector<std::vector<std::string>> g_cases;

char trump(const char move) {
    if (move == 'P') return 'S';
    if (move == 'S') return 'R';
    if (move == 'R') return 'P';
    return move;
}

int main(int argc, char* argv[])
{
    // parse console input
    int no_of_cases, no_of_players;
    std::cin >> no_of_cases;
    g_cases.resize(no_of_cases);
    for (int c = 0; c < no_of_cases; ++c) {
        std::cin >> no_of_players;
        auto& game = g_cases[c];
        game.resize(no_of_players);
        while (--no_of_players >= 0)
            std::cin >> game[no_of_players];
    }

    // padding
    for (auto& game : g_cases) {
        auto mm = std::minmax_element(game.begin(), game.end(), [](const auto& s1, const auto& s2) { return s1.size() < s2.size(); });
        if (mm.first != mm.second) {
            const int size = mm.second->size();
            for (auto& program : game) {
                const int short_size = program.size();
                for (int i = 0; program.size() < size; ++i)
                    program.push_back(program[i % short_size]);
            }
        }
    }

    // solution
    for (int c = 0; c < no_of_cases; ++c) {
        char next_move;
        int distinct_moves;
        bool tie{false};
        bool impossible{false};
        std::string solution;
        std::unordered_set<char> moves;
        const auto& game = g_cases[c];
        const auto len = game[0].size();
        for (int i = 0; i < len; ++i) {
            tie = false;
            moves.clear();
            for (auto& program : game) {
                const char opponent = program[i];
                moves.insert(opponent);
                distinct_moves = moves.size();
                next_move = trump(opponent);
                if (distinct_moves == 3) {
                    std::cout << "Case #" << (c + 1) << ": IMPOSSIBLE\n";
                    impossible = true;
                    i = len;
                    break;
                } else if (distinct_moves == 2) { // tie move
                    tie = true;
                    if (moves.count(next_move) == 0)
                        next_move = opponent;
                }
            }
        
            solution.push_back(next_move);

            if (!impossible && !tie) {
                std::cout << "Case #" << (c + 1) << ": " << solution << "\n";
                break;
            }
        }

        if (!impossible && tie)
            std::cout << "Case #" << (c + 1) << ": " << solution << "\n";
    }
}

/* clang++.exe -Wall -g -std=c++17 robot_strategy.cpp -o robot_strategy.exe

Input:

5
3
RR
PRS
PSRP
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

Output:

Case #1: PRRP
Case #2: PR
Case #3: P
Case #4: IMPOSSIBLE
Case #5: P

*/
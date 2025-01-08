/*
ID: marcin.8
LANG: C++
TASK: clocks
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=XgCDvqBWYNl&S=clocks
*/

#include <array>
#include <fstream>
#include <queue>
#include <string>

std::ifstream task_in("clocks.in");
std::ofstream task_out("clocks.out");

static constexpr const int MOVES = 9;
static const std::string moves[MOVES] = {"ABDE", "ABC", "BCEF", "ADG", "BDEFH", "CFI", "DEGH", "GHI", "EFHI"};

using state_t = std::array<int8_t, MOVES>;
static const state_t FINAL{};

struct node_t {
    state_t state;
    std::string path;  // moves sequence leading from init to this state
    int last_repeated; // how many times last move was repeated
};

/**
 * Observation1: Solving moves can be executed in any order
 * Observation2: Repeating a move more than 3 times makes no sense
 * Observation3: To solve in constant time, precompute moves to advance each single clock and cumulate counts modulo 4
 */
int main(int, char**)
{
    state_t init;
    for (int row = 0; row < 3; ++row)
        for (int col = 0; col < 3; ++col) {
            int clock; task_in >> clock;
            init[row * 3 + col] = clock / 3 % 4;
        }

    std::queue<node_t> q;
    q.push(node_t{init, {}, 0});
    while (true) {
        const auto& cur = q.front();
        if (cur.state == FINAL) {
            if (!cur.path.empty()) {
                const int l = int(cur.path.size() - 1);
                for (int p = 0; p < l; ++p)
                    task_out << cur.path[p] << ' ';
                task_out << cur.path.back();
            }
            task_out << '\n';
            return 0;
        }

        int next{};
        const bool repeat = cur.last_repeated < 3;
        if (!cur.path.empty()) {
            next = cur.path.back() - '1';
            if (!repeat)
                ++next;
        }

        for (int i = next; i < MOVES; ++i) {
            const auto& move = moves[i];
            state_t next_state = cur.state;
            for (char c : move)
                next_state[c - 'A'] = (next_state[c - 'A'] + 1) % 4;
            q.push(node_t{next_state, cur.path + char('1' + i), repeat && i == next ? cur.last_repeated + 1 : 1});
        }
        q.pop();
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 clocks.cpp -o clocks.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address clocks.cpp -o clocks

Run:
clocks.exe && type clocks.out

Input:


Output:


*/

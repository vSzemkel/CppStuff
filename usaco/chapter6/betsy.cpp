/*
ID: marcin.8
LANG: C++
TASK: betsy
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=kqiaO7Hw9Oq&S=betsy
*/

#include <bit>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <unordered_map>
#include <utility>
#include <vector>

std::ifstream task_in("betsy.in");
std::ofstream task_out("betsy.out");

int N, N2, STOP, HALFWAY, MIDPOINT, MEMO_LIMIT{45};
uint64_t FULL, ONE{1};

std::vector<std::unordered_map<uint64_t, int>> memo; // [pos][already visited] = count

//int popcount(uint64_t x)
//{
//    int ret{};
//    while (x) { x &= x - 1; ++ret; }
//    return ret;
//}

/**
 * Observation1: If Betsy hits a board boundary, she may isolate later inaccesible fragment.
 * Observation2: If she hits already visited cell, and there are two ways to turn available, one of them is a dead end.
 * Observation3: For odd N, the midpoint is a special case, every shorter path to midpoint has its longer, symmetric counterpart.
 * Observation4: To avoid checking destination cell on each move, we can mark it as visited and check only if we can 
 *               reach it from the last cell visited after visiting all the cells. This check is always positive!
 * Observation5: Caching is not very effective, but it is still worth it for few last moves.
 */
int betsy(const int pos, const uint64_t visited)
{
    if (visited == FULL) {
        assert (pos == STOP + 1 || pos == STOP - N); // 4
        return 1;
    }

    if (pos == MIDPOINT && (N == 7) && std::popcount(visited) < HALFWAY) // 3
        return 0;

    if (pos > MEMO_LIMIT && memo[pos - MEMO_LIMIT].count(visited)) // 5
        return memo[pos - MEMO_LIMIT][visited];

    int ways{}, avail{}, options[3];
    for (const int move : {-N, -1, 1, N}) {
        const auto can = pos + move;
        const auto bit = ONE << can;
        const auto nvis = visited | bit;
        if (visited ^ nvis) {
            const auto col = pos % N;
            if (0 <= can && can < N2 && (0 < col || move != -1) && (col < N - 1 || move != 1)) {
                ++avail;
                if (can < N && move == -N && !(visited & (ONE << (can - 1)))) // 1
                    continue;
                if (STOP < can && can < N2 - 1 && move == N && !(visited & (ONE << (can + 1)))) // 1
                    continue;
                if (can > N && !(visited & (ONE << (can - N)))) { // 1
                    const auto ncol = can % N;
                    if (ncol == 0 && move == -1)
                        continue;
                    if (ncol == N - 1 && move == 1)
                        continue;
                }
                options[ways++] = move;
            }
        }
    }

    if (ways == 0 || (ways == 2 && avail == 2 && (options[0] + options[1] == 0))) // 2
        return 0;

    int ans{};
    for (int m = 0; m < ways; ++m) {
        const auto can = pos + options[m];
        ans += betsy(can, visited | (ONE << can)); // 4
    }

    if (pos > MEMO_LIMIT)
        memo[pos - MEMO_LIMIT][visited] = ans; // 5

    return ans;
}

int main(int, char**)
{
    task_in >> N;
    N2 = N * N;
    STOP = N2 - N;
    HALFWAY = N2 / 2 + 2;
    MIDPOINT = N * (N / 2);
    if (N2 > MEMO_LIMIT)
        memo.resize(N2 - MEMO_LIMIT);
    FULL = ((ONE << N2) - 1);
    auto score = betsy(0, (ONE << STOP) + 1);
    if (N == 7) // 3
        score *= 2;
    task_out << std::max(1,score) << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++20 betsy.cpp -o betsy.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++20 -fsanitize=address betsy.cpp -o betsy

Run:
betsy.exe && type betsy.out

Input:

7

Output:

88418

*/

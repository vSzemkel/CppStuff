/*
ID: marcin.8
LANG: C++
TASK: betsy
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=kqiaO7Hw9Oq&S=betsy
*/

#include <bit>
#include <fstream>
#include <unordered_map>
#include <utility>
#include <vector>

std::ifstream task_in("betsy.in");
std::ofstream task_out("betsy.out");

int N, N2, STOP, HALFWAY, MIDPOINT, MEMO_LIMIT{40};
uint64_t FULL, ONE{1};

std::vector<std::unordered_map<uint64_t, int>> memo; // [pos][already visited] = count

int betsy(const int pos, const uint64_t visited)
{
    if (visited == FULL)
        return 1;

    if (pos == MIDPOINT && (N & 1) && std::popcount(visited) < HALFWAY)
        return 0;

    if (pos > MEMO_LIMIT && memo[pos - MEMO_LIMIT].count(visited))
        return memo[pos - MEMO_LIMIT][visited];

    int ans{};
    for (const int move : {-N, -1, 1, N}) {
        const auto can = pos + move;
        const auto bit = ONE << can;
        const auto nvis = visited | bit;
        if (visited ^ nvis) {
            const auto col = pos % N;
            if (0 <= can && can < N2 && (0 < col || move != -1) && (col < N - 1 || move != 1)) {
                if (can < N && move == -N && !(visited & (ONE << (can - 1))))
                    continue;
                if (STOP < can && can < N2 - 1 && move == N && !(visited & (ONE << (can + 1))))
                    continue;
                if (can > N && !(visited & (ONE << (can - N)))) {
                    const auto ncol = can % N;
                    if (ncol == 0 && move == -1)
                        continue;
                    if (ncol == N - 1 && move == 1)
                        continue;
                }
                ans += betsy(can, nvis);
            }
        }
    }

    if (pos > MEMO_LIMIT)
        memo[pos - MEMO_LIMIT][visited] = ans;

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
    if (N & 1)
        score *= 2;
    task_out << std::max(1,score) << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 betsy.cpp -o betsy.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address betsy.cpp -o betsy

Run:
betsy.exe && type betsy.out

Input:

7

Output:

88418

*/

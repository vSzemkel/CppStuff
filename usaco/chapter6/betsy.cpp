/*
ID: marcin.8
LANG: C++
TASK: betsy
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=kqiaO7Hw9Oq&S=betsy
*/

#include <fstream>
#include <unordered_map>
#include <utility>
#include <vector>

std::ifstream task_in("betsy.in");
std::ofstream task_out("betsy.out");

int N, N2, STOP;
uint64_t FULL, ONE{1};

std::vector<std::unordered_map<uint64_t, int>> memo; // [pos][already visited] = count

int betsy(const int pos, const uint64_t visited)
{
    if (visited == FULL)
        return 1;

    if (memo[pos].count(visited))
        return memo[pos][visited];

    int ans{};
    for (const int move : {-N, -1, 1, N}) {
        const auto can = pos + move;
        const auto bit = ONE << can;
        const auto nvis = visited | bit;
        if (!(visited & bit) && (can != STOP || nvis == FULL)) {
            const auto col = pos % N;
            if (0 <= can && can < N2 && (0 < col || move != -1) && (col < N - 1 || move != 1))
                ans += betsy(can, nvis);
        }
    }

    return memo[pos][visited] = ans;
}

int main(int, char**)
{
    task_in >> N;
    N2 = N * N;
    STOP = N2 - N;
    memo.resize(N2);
    FULL = (ONE << N2) - 1;
    task_out << betsy(0, 1) << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 betsy.cpp -o betsy.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address betsy.cpp -o betsy

Run:
betsy.exe && type betsy.out

Input:

6

Output:

1770

*/

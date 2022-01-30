/*
ID: marcin.8
LANG: C++
TASK: game1
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=qyzwTp4qKKA&S=game1
*/

#include <fstream>
#include <utility>
#include <vector>

std::ifstream task_in("game1.in");
std::ofstream task_out("game1.out");

int main(int, char**)
{
    int N; task_in >> N;
    std::vector<int> game(N);
    for (auto& n : game)
        task_in >> n;

    std::vector<std::vector<std::pair<int, int>>> dp(N + 1, std::vector<std::pair<int, int>>(N)); // dp[k][l] == {score P1, score P2} for subproblem starting at position k with length l
    for (int pos = 0; pos < N; ++pos)
        dp[1][pos] = {game[pos], 0};
    for (int len = 2; len <= N; ++len) {
        const auto& prev = dp[len - 1];
        for (int pos = 0; pos <= N - len; ++pos) {
            const int left = game[pos] + prev[pos + 1].second;
            const int right = game[pos + len - 1] + prev[pos].second;
            if (left < right)
                dp[len][pos] = { right, prev[pos].first };
            else
                dp[len][pos] = { left, prev[pos + 1].first };
        }
    }

    auto& ans = dp[N][0];
    task_out << ans.first << ' ' << ans.second << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 game1.cpp -o game1.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address game1.cpp -o game1

Input:

30 
105 57 64 79 188 102 52 145 188 2 163 147 174 163 198 
9 133 159 184 104 70 19 57 174 184 57 154 123 106 129

Output:

2043 1446

*/

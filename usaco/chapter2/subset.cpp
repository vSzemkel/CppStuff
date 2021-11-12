/*
ID: marcin.8
LANG: C++
TASK: subset
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=rsMd19gDe0C&S=subset
*/

#include <fstream>
#include <vector>

std::ifstream task_in("subset.in");
std::ofstream task_out("subset.out");

int acceptable(int, char**) 
{
    int N; task_in >> N;
    const int MAX = N * (N + 1) / 2;
    // dp[i][j] - number of partitions of i elements with difference j; dp[N][0] is the answer
    std::vector<std::vector<int>> dp(N + 1, std::vector<int>(MAX + 1));
    dp[1][1] = 1;
    for (int n = 2; n <= N; ++n) {
        for (int diff = 0; diff <= MAX; ++diff) {
            dp[n][std::abs(diff - n)] += dp[n - 1][diff];
            dp[n][n + diff] += dp[n - 1][diff];
        }
    }

    task_out << dp[N][0] << '\n';
}

int main(int, char**) 
{
    int N; task_in >> N;
    const int MAX = N * (N + 1);
    // dp[i] - number of partitions with sum equal i
    if ((MAX % 4) == 0) {
        const int sum = MAX / 4;
        std::vector<int64_t> dp(sum + 1);
        dp[0] = 1;
        for (int n = 1; n <= N; ++n)
            for (int s = sum; s >= n; --s)
                dp[s] += dp[s - n];

        task_out << (dp[sum] / 2) << '\n';
    } else
        task_out << "0\n";
}
/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 subset.cpp -o subset.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address subset.cpp -o subset

Input:

15

Output:

361

*/

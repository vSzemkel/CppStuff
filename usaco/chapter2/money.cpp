/*
ID: marcin.8
LANG: C++
TASK: money
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=oUjOlLv4ixa&S=money
*/

#include <fstream>
#include <vector>

std::ifstream task_in("money.in");
std::ofstream task_out("money.out");

int main(int, char**)
{
    int V, N; task_in >> V >> N;
    std::vector<int> coins(V);
    for (auto& c : coins)
        task_in >> c;

    std::vector<int64_t> solution(N + 1);
    solution[0] = 1;
    for (const auto& c : coins)
        for (int sum = c; sum <= N; ++sum)
            // if (solution[sum - c] > 0) - if found, propagate, check not needed
                solution[sum] += solution[sum - c];

    task_out << solution[N] << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 money.cpp -o money.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address money.cpp -o money

Input:

17 500 
11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27

Output:

18390132498

*/

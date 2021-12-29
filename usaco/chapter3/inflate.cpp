/*
ID: marcin.8
LANG: C++
TASK: inflate
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=mVCjUhF8Kba&S=inflate
*/

#include <algorithm>
#include <fstream>
#include <vector>

std::ifstream task_in("inflate.in");
std::ofstream task_out("inflate.out");

int main(int, char**)
{
    int T, N, cost, time;
    task_in >> T >> N;
    std::vector<int> scores(T + 1);
    for (int c = 0; c < N; ++c) {
        task_in >> cost >> time;
        for (int t = time; t <= T; ++t)
            scores[t] = std::max(scores[t], scores[t - time] + cost);
    }

    task_out << scores[T] << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 inflate.cpp -o inflate.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address inflate.cpp -o inflate

Input:

300 4
100 60
250 120
120 100
35 20

Output:

605

*/

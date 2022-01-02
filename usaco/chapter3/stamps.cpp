/*
ID: marcin.8
LANG: C++
TASK: stamps
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=1zOdKFAST7f&S=stamps
*/

#include <algorithm>
#include <assert.h>
#include <fstream>
#include <vector>

std::ifstream task_in("stamps.in");
std::ofstream task_out("stamps.out");

int main(int, char**)
{
    int K, N; task_in >> K >> N;
    std::vector<int> stamps(N);
    for (auto& s : stamps)
        task_in >> s;

    const auto mM = std::minmax_element(stamps.begin(), stamps.end());
    assert(*mM.first == 1);

    const int INF = 1e09;
    const int MAX = K * (*mM.second);
    std::vector<int> req(MAX + 1, INF);
    for (const auto s : stamps)
        req[s] = 1;
    int ret{2};
    while (ret <= MAX) {
        for (const auto s : stamps)
            if (0 < ret - s && 0 < req[ret - s] && req[ret - s] < K)
                req[ret] = std::min(req[ret], req[ret - s] + 1);
        if (req[ret] == INF)
            break;
        ++ret;
    }

    task_out << ret - 1 << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 stamps.cpp -o stamps.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address stamps.cpp -o stamps

Input:

5 2
1 3

Output:

13

*/

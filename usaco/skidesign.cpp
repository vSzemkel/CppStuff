/*
ID: marcin.8
LANG: C++
TASK: skidesign
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=ZSnHrkNl8Io&S=skidesign
*/

#include <algorithm>
#include <cmath>
#include <fstream>
#include <vector>

std::ifstream task_in("skidesign.in");
std::ofstream task_out("skidesign.out");

constexpr const int WIDTH = 17;

int main(int, char**)
{
    int N; task_in >> N;
    std::vector<int> hills(N);
    for (auto& h : hills)
        task_in >> h;

    std::sort(hills.begin(), hills.end());
    const int lo = hills.front(), hi = hills.back();

    int64_t ret{INT64_MAX};
    for (int lbound = lo, rbound = lo + WIDTH; rbound <= hi; ++lbound, ++rbound) {
        int pos{0};
        int64_t cur{0};
        while (hills[pos] < lbound) {
            const auto diff = std::ceil(lbound - hills[pos]);
            cur += diff * diff;
            ++pos;
        }
        pos = N - 1;
        while (rbound < hills[pos]) {
            const auto diff = std::ceil(hills[pos] - rbound);
            cur += diff * diff;
            --pos;
        }
        ret = std::min(ret, cur);
    }

    task_out << ret << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 skidesign.cpp -o skidesign.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address skidesign.cpp -o skidesign

Input:

5
20
4
1
24
21

Output:

18

*/

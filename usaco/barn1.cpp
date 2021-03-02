/*
ID: marcin.8
LANG: C++
TASK: barn1
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=10GR0PhXOrT&S=barn1
*/

#include <cassert>
#include <iostream>
#include <fstream>
#include <queue>
#include <utility>
#include <vector>

std::ifstream task_in("barn1.in");
std::ofstream task_out("barn1.out");

int main(int, char**)
{
    int M; // maximal numer of boards
    int S; // total number of stalls
    int C; // total number of cows
    task_in >> M >> S >> C;
    std::vector<bool> stalls(S);
    for (int i = 0; i < C; ++i) {
        int offset; task_in >> offset; --offset;
        stalls[offset] = true;
    }

    assert(C > 0);
    int lbound{0};
    while (!stalls[lbound]) ++lbound;
    int rbound{S - 1};
    while (!stalls[rbound]) --rbound;
    ++rbound;

    std::priority_queue<int> pq; // {gap_length}
    int lgap{lbound};
    while (true) {
        while (lgap < rbound && stalls[lgap]) ++lgap;
        int rgap{lgap};
        while (rgap < rbound && !stalls[rgap]) ++rgap;
        if (lgap < rgap && rgap < rbound)
            pq.push(rgap - lgap);
        else
            break;
        lgap = rgap;
    }

    int max_gap = M - 1;
    int ret{rbound - lbound};
    while (max_gap > 0 && !pq.empty()) {
        const int gap = pq.top();
        ret -= gap;
        pq.pop();
        --max_gap;
    }

    task_out << ret << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 barn1.cpp -o barn1.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 barn1.cpp -o barn1.o

Input:

4 50 18
3
4
6
8
14
15
16
17
21
25
26
27
30
31
40
41
42
43

Output:

25

*/

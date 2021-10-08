/*
ID: marcin.8
LANG: C++
TASK: hamming
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=TaWHBDmYDNq&S=hamming
*/

#include <algorithm>
#include <fstream>
#include <vector>

std::ifstream task_in("hamming.in");
std::ofstream task_out("hamming.out");

constexpr const int LINE = 10;

int main(int, char**)
{
    int N, B, D;
    task_in >> N >> B >> D;

    int val{0};
    std::vector<int> trace;
    for (int c = 0; c < N; ) {
        while (!std::all_of(trace.begin(), trace.end(), [=](const int n){ return __builtin_popcount(n ^ val) >= D; }))
            ++val;
        trace.push_back(val);
        task_out << val << ((++c % LINE) == 0 || c == N ? '\n' : ' ');
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 hamming.cpp -o hamming.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address hamming.cpp -o hamming

Input:

16 7 3

Output:

0 7 25 30 42 45 51 52 75
76 82 85 97 102 120

*/

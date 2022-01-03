/*
ID: marcin.8
LANG: C++
TASK: fact4
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=L1O7vSeArjL&S=fact4
*/

#include <cassert>
#include <fstream>

std::ifstream task_in("fact4.in");
std::ofstream task_out("fact4.out");

/**
 * @brief Alternative: count 2 i 5 factors in multiplicants, compute last nonzero
 * digit modulo 10 and next multiply it by 2mod10 n2-n5 times
 */
int main(int, char**)
{
    int N; task_in >> N;

    int ret{1};
    for (int i = 2; i <= N; ++i) {
        ret *= i;
        while ((ret % 10) == 0) 
            ret /= 10;
        ret %= 10000;
    }

    task_out << ret % 10 << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 fact4.cpp -o fact4.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address fact4.cpp -o fact4

Input:

143

Output:

4

*/

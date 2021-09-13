/*
ID: marcin.8
LANG: C++
TASK: numtri
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=4odFR5Ojcpb&S=numtri
*/

#include <algorithm>
#include <fstream>
#include <vector>

std::ifstream task_in("numtri.in");
std::ofstream task_out("numtri.out");

int main(int, char**)
{
    std::vector<int> triangle;
    int N; task_in >> N;
    const int size = N * (N + 1) / 2;
    triangle.resize(size);
    for (auto& n : triangle)
        task_in >> n;

    for (int r = N - 1; r; --r)
        for (int c = r * (r - 1) / 2, z = r; z; ++c, --z)
            triangle[c] += std::max(triangle[c + r], triangle[c + r + 1]);

    task_out << triangle[0] << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 numtri.cpp -o numtri.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address numtri.cpp -o numtri

Input:

5
7
3 8
8 1 0
2 7 4 4
4 5 2 6 5

Output:

30

*/

/*
ID: marcin.8
LANG: C++
TASK: fence9
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=1hifHvFsbU1&S=fence9
*/

#include <fstream>
#include <numeric>

std::ifstream task_in("fence9.in");
std::ofstream task_out("fence9.out");

/**
 * @see https://en.wikipedia.org/wiki/Pick's_theorem
 * A = I + B/2 - 1
 */
int main(int, char**)
{
    int N, M, P;
    task_in >> N >> M >> P;

    const auto points_on_section = [](const int w, const int h){
        if (w == 0) return h;
        if (h == 0) return w;
        return std::gcd(w, h);
    };

    const int area2 = P * M;
    const int boundary = points_on_section(0, P) + points_on_section(N, M)
                       + points_on_section(std::abs(P - N), M);
    task_out << (area2 - boundary) / 2 + 1 << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 fence9.cpp -o fence9.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address fence9.cpp -o fence9

Input:

100 200 50

Output:

4901

*/

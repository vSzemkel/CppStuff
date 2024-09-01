/*
ID: marcin.8
LANG: C++
TASK: twofive
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=FIktgbZZPYQ&S=twofive
*/

#include <cassert>
#include <fstream>
#include <string>

std::ifstream task_in("twofive.in");
std::ofstream task_out("twofive.out");

const int ROWS = 5;
static std::string pattern = "ABCDEFGHIJKLMNOPQRSTUVWXY";

std::string num2str()
{
    int N;
    task_in >> N;
    assert(--N < (1 << (ROWS - 1)));

    for (int r = ROWS - 1; N; --r, N >>= 1)
        if (N & 1) {
            const auto cell = r * ROWS;
            std::swap(pattern[cell - 1], pattern[cell]);
        }

    return pattern;
}

std::string str2num()
{
    std::string inp;
    task_in >> inp;
    assert(pattern.size() == ROWS * ROWS);

    int n{};
    for (int r = 1; r < ROWS; ++r) {
        n <<= 1;
        const auto cell = r * ROWS;
        if (inp[cell - 1] > inp[cell])
            ++n;
    }

    return std::to_string(++n);
}

int main(int, char**)
{
    assert(pattern.size() == ROWS * ROWS);

    char mode;
    task_in >> mode >> std::ws;
    task_out << (mode == 'N' ? num2str() : str2num()) << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 twofive.cpp -o twofive.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address twofive.cpp -o twofive

Run:
twofive.exe && type twofive.out

Input:


Output:


*/

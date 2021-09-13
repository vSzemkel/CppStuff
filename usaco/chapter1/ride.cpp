/*
ID: marcin.8
LANG: C++
TASK: ride
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=F3EJ5Ggwb1V&S=ride
*/

#include <iostream>
#include <fstream>

std::ifstream task_in("ride.in");
std::ofstream task_out("ride.out");

uint64_t decrypt(const std::string& s)
{
    uint64_t ret{1};
    for (const char c : s)
        ret *= (c - 'A' + 1);
    return ret % 47;
}

int main(int, char**)
{
    std::string comet, group;
    task_in >> comet >> group;
    task_out << (decrypt(comet) == decrypt(group) ? "GO" : "STAY") << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 ride.cpp -o ride.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 ride.cpp -o ride.o

Input:

COMETQ
HVNGAT

Output:

GO

*/

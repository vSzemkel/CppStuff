/*
ID: marcin.8
LANG: C++
TASK: milk
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=10GR0PhXOrT&S=milk
*/

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>

std::ifstream task_in("milk.in");
std::ofstream task_out("milk.out");

int main(int, char**)
{
    std::string comet, group;
    int total, N; task_in >> total >> N;
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
    for (int i = 0; i < N; ++i) {
        int price, resource; task_in >> price >> resource;
        pq.push({price, resource});
    }

    int ret{0};
    while (total > 0) {
        const auto& milkman = pq.top();
        const int quantity = std::min(total, milkman.second);
        total -= quantity;
        ret += quantity * milkman.first;
        pq.pop();
    }

    task_out << ret << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 milk.cpp -o milk.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 milk.cpp -o milk.o

Input:

100 5
5 20
9 40
3 10
8 80
6 30

Output:

630

*/

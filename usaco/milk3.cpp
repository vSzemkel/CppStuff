/*
ID: marcin.8
LANG: C++
TASK: milk3
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=jQSbbGN9Z6f&S=milk3
*/

#include <algorithm>
#include <array>
#include <fstream>
#include <set>
#include <vector>

std::ifstream task_in("milk3.in");
std::ofstream task_out("milk3.out");

constexpr const int MAX = 20;

std::array<int, 3> LIMITS;
std::vector<std::array<int, 3>> pending;
std::vector<bool> config(MAX * (MAX * MAX + MAX + 1));

static int checksum(const std::array<int, 3>& c) {
    return c[0] * MAX * MAX + c[1] * MAX + c[2];
}

static void pour(const std::array<int, 3>& c, const int from, const int to) {
    if (c[from] == 0 || c[to] == LIMITS[to])
        return;
    const int amount = std::min(c[from], LIMITS[to] - c[to]);
    auto next = c;
    next[from] -= amount;
    next[to] += amount;

    const int csum = checksum(next);
    if (!config[csum]) {
        config[csum] = true;
        pending.push_back(next);
    }
}

int main(int, char**)
{
    task_in >> LIMITS[0] >> LIMITS[1] >> LIMITS[2];

    std::set<int> ret;
    config[LIMITS[2]] = true;
    pending.push_back({0, 0, LIMITS[2]});
    while (!pending.empty()) {
        const auto c = pending.back();
        pending.pop_back();
        if (c[0] == 0)
            ret.insert(c[2]);
        else {
            pour(c, 0, 1);
            pour(c, 0, 2);
        }

        pour(c, 1, 0);
        pour(c, 1, 2);
        pour(c, 2, 0);
        pour(c, 2, 1);
    }

    for (const int r : ret)
        task_out << r << ' ';
    task_out.seekp(-1, std::ios_base::cur);
    task_out << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 milk3.cpp -o milk3.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address milk3.cpp -o milk3

Input:

8 9 10

Output:

1 2 8 9 10

*/

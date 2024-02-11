
#include <algorithm>
#include <array>
#include <format>
#include <iostream>

// Activity selection
// Given a set of activities with [start, finish) times schedule 
// maximal number non overlapping activities

constexpr const int N = 13;

std::array<std::array<int, 2>, N> activities = {
    std::array<int, 2>{0, 4},
    {3, 6},
    {5, 6},
    {0, 10},
    {0, 5},
    {6, 8},
    {4, 5},
    {8, 10},
    {2, 5},
    {4, 8},
    {1, 3},
    {6, 7},
    {9, 10},
};

/**
 * @brief This is a kind of problem with optimal substructure
 * but with greedy approach we can do better then with DP
 */
int main(int, char**)
{
    std::sort(activities.begin(), activities.end());
    int next_start{}, ord{0};
    for (const auto& [start, stop] : activities)
        if (next_start <= start) {
            std::cout << std::format("{}. [{}, {})\n", ++ord, start, stop);
            next_start = stop;
        }

    std::cout << "Maximum " << ord << " activities can be arranged\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 activity_selection.cpp -o activity_selection.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++20 -fsanitize=address activity_selection.cpp -o activity_selection

Output:

Maximum 4 activities can be arranged
1. [1, 3)
2. [4, 5)
3. [5, 6)
4. [6, 8)
5. [9, 10)

*/

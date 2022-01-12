
#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

// Activity selection
// Given a set of activities with [start, finish) times schedule 
// maximal number non overlapping activities

constexpr const int N = 12;

std::array<std::array<int, 2>, N> activities = {
    std::array<int, 2>{0, 4},
    {3, 6},
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
    std::vector<int> choosen(1, N - 1);
    int next_start = activities.back()[0];
    for (int a = N - 2; a >= 0; --a)
        if (activities[a][1] <= next_start) {
            choosen.push_back(a);
            next_start = activities[a][0];
        }

    const int ans = int(choosen.size());
    std::cout << "Maximum " << ans << " activities can be arranged\n";
    for (int i = ans - 1, c = 1; i >= 0; --i, ++c) {
        const auto& act = activities[choosen[i]];
        std::cout << c << ". [" << act[0] << ", " << act[1] << ")\n";
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 activity_selection.cpp -o activity_selection.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address activity_selection.cpp -o activity_selection

Output:

Maximum 4 activities can be arranged
1. [1, 3)
2. [4, 5)
3. [6, 8)
4. [9, 10)

*/

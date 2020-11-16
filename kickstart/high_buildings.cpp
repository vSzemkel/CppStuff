
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <utility>
#include <vector>

// High Buildings
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ff47/00000000003bef73

std::vector<int> g_input;
size_t g_size;

void solve(int a, int b, const int c) {
    assert(a >= c && b >= c && g_size >= c);

    bool reverse{false};
    if (a > b) {
        reverse = true;
        std::swap(a, b);
    }

    int pos{a - c};
    g_input.assign(g_size, 1);
    for (int i = 0; i < c; ++i) g_input[pos++] = g_size;
    const int diff = g_size - pos + c - b; // B see diff many buildings too much
    if (0 < diff && pos + diff < g_size)
        g_input[pos + diff] = g_size - 1;
    else if (pos + diff == g_size && c > 1)
        std::swap(g_input[pos - 1], g_input[g_size - 1]);
    else if (diff != 0) {
        std::cout << "IMPOSSIBLE";
        return;
    }

    if (reverse)
        std::reverse(g_input.begin(), g_input.end());

    for (auto& h : g_input)
        std::cout << h << " ";
}

int main(int argc, char* argv[])
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    // parse console input
    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        int a, b, c;
        std::cin >> g_size >> a >> b >> c;
        std::cout << "Case #" << g << ": ";
        solve(a, b, c);
        std::cout << "\n";
    }
}

/*
clang++.exe -Wall -ggdb3 -O0 -std=c++17 high_buildings.cpp -o high_buildings.exe
g++ -Wall -ggdb3 -O0 -std=c++14 high_buildings.cpp -o high_buildings.o
high_buildings.exe < high_buildings.in

Input:

7
10 1 1 1
10 2 2 2
10 5 2 2
4 1 3 1
4 4 4 3
5 3 3 2
1 1 1 1

Output:

Case #1: IMPOSSIBLE
Case #2: 10 1 1 1 1 1 1 1 1 10
Case #3: 1 1 9 1 1 1 1 1 10 10
Case #4: 4 1 3 1 
Case #5: IMPOSSIBLE
Case #6: 1 5 5 1 4
Case #7: 1

*/


#include <algorithm>
#include <iostream>
#include <numeric>
#include <stdlib.h>
#include <vector>

// Rugby
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ff49/000000000043b027

size_t g_size;
std::vector<int64_t> g_x, g_y;

int64_t solve() {
    const int64_t mid = g_size >> 1;
    // make it horizontally
    std::nth_element(g_y.begin(), g_y.begin() + mid, g_y.end());
    const int64_t cost_y = std::accumulate(g_y.begin(), g_y.end(), 0LL, [t = g_y[mid]](const int64_t acu, const int64_t y){
        return acu + abs(t - y);
    });
    // set proper horizontal row
    std::sort(g_x.begin(), g_x.end());
    for (int i = 0; i < g_size; ++i)
        g_x[i] -= i;
    std::nth_element(g_x.begin(), g_x.begin() + mid, g_x.end());
    const int64_t cost_x = std::accumulate(g_x.begin(), g_x.end(), 0LL, [t = g_x[mid]](const int64_t acu, const int64_t x){
        return acu + abs(t - x);
    });

    return cost_x + cost_y;
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
        std::cin >> g_size;
        g_x.resize(g_size);
        g_y.resize(g_size);
        for (int i = 0; i < g_size; ++i)
            std::cin >> g_x[i] >> g_y[i];

        std::cout << "Case #" << g << ": " << solve() << "\n";
    }
}

/*
clang++.exe -Wall -ggdb3 -O0 -std=c++17 rugby.cpp -o rugby.exe
g++ -Wall -ggdb3 -O0 -std=c++14 rugby.cpp -o rugby.o
rugby.exe < rugby.in

Input:

6
2
1 1
4 4
3
1 1
1 2
1 3
4
1 1
1 2
2 1
2 2
5
0 0
5 0
10 0
15 0
23 0
4
1 0
1 1
1 10
1 100
6
1 -12
3 41
13 541
31 0
1000 91
1050 6

Output:

Case #1: 5
Case #2: 4
Case #3: 4
Case #4: 27
Case #5: 113
Case #6: 2734

*/

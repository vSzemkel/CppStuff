
#include <algorithm>
#include <iostream>
#include <vector>

// Allocation
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ffc7/00000000001d3f56


static void solve() {
    int N, B; std::cin >> N >> B;
    std::vector<int> houses(N);
    for (int i = 0; i < N; ++i)
        std::cin >> houses[i];

    int ret{0};
    std::sort(houses.begin(), houses.end());
    while (B > 0 && ret < N) {
        B -= houses[ret];
        if (B >= 0) ++ret;
    }

    std::cout << ret;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 allocation.cpp -o allocation.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address allocation.cpp -o allocation.o

Run:
py.exe interactive_runner.py py.exe allocation_testing_tool.py 1 -- allocation.exe
allocation.exe < allocation.in

Input:

3
4 100
20 90 40 90
4 50
30 30 10 10
3 300
999 999 999

Output:

Case #1: 2
Case #2: 3
Case #3: 0

*/


#include <algorithm>
#include <iostream>
#include <vector>

// Bus routes
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ffc8/00000000002d83bf


static void solve() {
    int N; int64_t D; std::cin >> N >> D;
    std::vector<int64_t> buses(N);
    for (int i = 0; i < N; ++i)
        std::cin >> buses[i];
    std::reverse(buses.begin(), buses.end());

    for (int i = 0; i < N; ++i) {
        D -= D % buses[i];
    }
    
    std::cout << D;
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
        //std::cout << "Case #" << g << ": " << std::setprecision(15) << solve() << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 bus_routes.cpp -o bus_routes.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address bus_routes.cpp -o bus_routes.o

Run:
py.exe interactive_runner.py py.exe bus_routes_testing_tool.py 1 -- bus_routes.exe
bus_routes.exe < bus_routes.in

Input:

3
3 10
3 7 2
4 100
11 10 5 50
1 1
1

Output:

Case #1: 6
Case #2: 99
Case #3: 1

*/

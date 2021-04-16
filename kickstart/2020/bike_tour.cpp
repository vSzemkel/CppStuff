
#include <iostream>
#include <vector>

// Bike tour
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ffc8/00000000002d82e6


static void solve() {
    int N; std::cin >> N;
    std::vector<int> hills(N);
    for (int i = 0; i < N; ++i)
        std::cin >> hills[i];

    int64_t ret{0};
    for (int i = 1; i < N - 1; ++i)
        if (hills[i - 1] < hills[i] && hills[i] > hills[i + 1])
            ++ret;

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
        //std::cout << "Case #" << g << ": " << std::setprecision(15) << solve() << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 bike_tour.cpp -o bike_tour.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address bike_tour.cpp -o bike_tour.o

Run:
py.exe interactive_runner.py py.exe bike_tour_testing_tool.py 1 -- bike_tour.exe
bike_tour.exe < bike_tour.in

Input:

4
3
10 20 14
4
7 7 7 7
5
10 90 20 90 10
3
10 3 10

Output:

Case #1: 1
Case #2: 0
Case #3: 2
Case #4: 0

*/

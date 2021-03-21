
#include <iostream>
#include <utility>

// Retype
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ff49/000000000043adc7

int64_t solve(const int n, const int k, const int s) {
    return n + std::min(2 * (k - s), k);
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
        int n, k, s;
        std::cin >> n >> k >> s;
        std::cout << "Case #" << g << ": " << solve(n, k, s) << "\n";
    }
}

/*
clang++.exe -Wall -ggdb3 -O0 -std=c++17 retype.cpp -o retype.exe
g++ -Wall -ggdb3 -O0 -std=c++14 retype.cpp -o retype.o
retype.exe < retype.in

Input:

2
10 5 2
10 7 6

Output:

Case #1: 15
Case #2: 12

*/

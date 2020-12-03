
#include <iostream>
#include <string>

// Nesting Depth
// https://codingcompetitions.withgoogle.com/codejam/round/000000000019fd27/0000000000209a9f


void solve() {
    std::string text;
    std::cin >> text;
    int level{0};
    for (const char c : text) {
        int val = c - '0';
        while (level < val) {
            std::cout << '(';
            ++level;
        }
        while (level > val) {
            std::cout << ')';
            --level;
        }
        std::cout << c;
        level = val;
    }

    while (level > 0) {
        std::cout << ')';
        --level;
    }
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << "\n";
    }
}

/*
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 nesting_depth.cpp -o nesting_depth.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 nesting_depth.cpp -o nesting_depth.o
nesting_depth.exe < nesting_depth.in

Input:

5
0000
101
111000
1
0213124221

Output:

Case #1: 0000
Case #2: (1)0(1)
Case #3: (111)000
Case #4: (1)
Case #5: 0((2)1((3))1(2((4))22)1)

*/

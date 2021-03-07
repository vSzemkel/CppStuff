
#include <iostream>
#include <string>

// Foregone solution
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000051705/0000000000088231


static void solve() {
    std::string A, B, N; std::cin >> N;
    for (const char c : N) {
        if (c == '0') {
            A.push_back('0');
            B.push_back('0');
        } else if (c == '5') {
            A.push_back('2');
            B.push_back('3');
        } else {
            A.push_back('1');
            B.push_back(c - 1);
        }
    }

    std::cout << A << " " << B << '\n';
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve();
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 foregone_solution.cpp -o foregone_solution.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 foregone_solution.cpp -o foregone_solution.o

Run:
foregone_solution.exe < foregone_solution.in

Input:

4
4
940
4444
1000000

Output:

Case #1: 1 3
Case #2: 110 830
Case #3: 1111 3333
Case #4: 0000000 1000000

*/

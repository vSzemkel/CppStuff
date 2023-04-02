
#include <iostream>
#include <vector>

// Candies
// https://codeforces.com/contest/1810/problem/B

static void solve() {
    int N; std::cin >> N;
    if (N % 2 == 0)
        std::cout << -1;
    else {
        std::vector<bool> spells;
        while (N > 1) {
            if ((N / 2) % 2 == 1) {
                spells.push_back(true);
                N = N / 2;
            } else {
                spells.push_back(false);
                N = (N / 2) + 1;
            }
        }

        const int size = int(spells.size());
        std::cout << size << '\n';
        for (int z = size - 1; ~z; --z)
            std::cout << (spells[z] ? 2 : 1) << ' ';
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
        solve(); std::cout << '\n';
    }
}

/*

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++17 candies.cpp -o candies.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address candies.cpp -o candies

Run:
candies.exe < candies.in

Input:

4
2
3
7
17

Output:

-1
1
2 
2
2 2
4
2 1 1 1

*/

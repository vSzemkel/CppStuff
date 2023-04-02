
#include <iostream>

// Beautiful Sequence
// https://codeforces.com/contest/1810/problem/A

static void solve() {
    bool found{};
    int N; std::cin >> N;
    for (int i = 1; i <= N; ++i) {
        int a; std::cin >> a;
        found |= a <= i;
    }

    std::cout << (found ? "YES" : "NO");
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
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++17 beautiful_sequence.cpp -o beautiful_sequence.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address beautiful_sequence.cpp -o beautiful_sequence

Run:
beautiful_sequence.exe < beautiful_sequence.in

Input:

7
3
3 2 1
4
2 4 3 5
5
2 3 5 5 6
2
3 1
5
2 4 5 2 3
4
5 6 7 8
6
6 5 4 3 2 1

Output:

YES
YES
NO
YES
YES
NO
YES

*/

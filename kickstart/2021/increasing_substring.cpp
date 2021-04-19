
#include <iostream>
#include <string>
#include <vector>

// Increasing substring
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000435a5b/000000000077a882


static void solve() {
    int L; std::string s; std::cin >> L >> s;
    int pos{0};
    std::vector<int> data(L, 1);
    while (pos < L) {
        int end = pos + 1;
        while (end < L && s[end - 1] < s[end]) {
            data[end] = end - pos + 1;
            ++end;
        }
        pos = end;
    }

    for (int i = 0; i < L; ++i)
        std::cout << data[i] << " ";
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 increasing_substring.cpp -o increasing_substring.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address increasing_substring.cpp -o increasing_substring.o

Run:
increasing_substring.exe < increasing_substring.in

Input:

2
4
ABBC
6
ABACDA

Output:


*/

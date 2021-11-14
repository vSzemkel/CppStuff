
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// Painter
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000435914/00000000008d9a88


constexpr const int RR = 1;
constexpr const int YY = 2;
constexpr const int BB = 4;

static void solve() {
    int N; std::string S;
    std::cin >> N >> S;

    std::unordered_map<char, int> col;
    col['U'] = 0;
    col['R'] = 1;
    col['Y'] = 2;
    col['B'] = 4;
    col['O'] = 3;
    col['P'] = 5;
    col['G'] = 6;
    col['A'] = 7;

    std::vector<bool> R(N + 1), Y(N + 1), B(N + 1); // +1 for sentinel with default value false
    for (int i = 0; i < N; ++i) {
        const int c = col[S[i]];
        if (c & RR)
            R[i] = true;
        if (c & YY)
            Y[i] = true;
        if (c & BB)
            B[i] = true;
    }

    /*int64_t ret{0}; // human friendly logic
    for (const auto& sep : {R, Y, B}) {
        int pos{0};
        while (true) {
            while (pos < N && !sep[pos]) ++pos;
            if (pos == N) break;
            ++ret;
            while (pos < N && sep[pos] != 0) ++pos;
        }
    }

    std::cout << ret;*/

    int64_t alt{0};
    for (auto& sep : {R, Y, B}) { // this version requires sentinel
        for (int pos = 0; pos < N; ++pos)
            if (sep[pos] && !sep[pos + 1])
                ++alt;
    }

    std::cout << alt;
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 painter.cpp -o painter.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address painter.cpp -o painter

Run:
painter.exe < painter.in

Input:

3
9
YYYBBBYYY
6
YYGGBB
5
ROAOR

Output:

Case #1: 3
Case #2: 2
Case #3: 3

*/


#include <algorithm>
#include <iostream>
#include <string>

// Transform the string
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000435914/00000000008da461

constexpr const int LC = 26;

static void solve() { // O(SlogF) - simpler solution in O(SF) was acceptable
    std::string S, F;
    std::cin >> S >> F;

    int ret{0};
    for (const char c : S) {
        const auto it = std::lower_bound(F.begin(), F.end(), c);

        int can(0xff);
        if (it != F.end())
            can = *it - c;
        else
            can = F.front() + LC - c;

        if (it != F.begin())
            can = std::min(can, c - *(it - 1));
        else
            can = std::min(can, c - F.back() + LC);

        ret += can;
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 transform_the_string.cpp -o transform_the_string.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address transform_the_string.cpp -o transform_the_string

Run:
transform_the_string.exe < transform_the_string.in

Input:

3
pqrst
ou
abd
abd
aaaaaaaaaaaaaaab
aceg

Output:

Case #1: 9
Case #2: 0
Case #3: 1

*/

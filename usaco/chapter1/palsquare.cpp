/*
ID: marcin.8
LANG: C++
TASK: palsquare
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=V6Wf7bpcFvl&S=palsquare
*/

#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>

std::ifstream task_in("palsquare.in");
std::ofstream task_out("palsquare.out");

constexpr int max = 300;
constexpr char digits[21] = "0123456789ABCDEFGHIJ";

std::string convert(int n, const int base)
{
    assert(1 < base && base < 21);
    std::string ret;
    while (n > 0) {
        ret.push_back(digits[n % base]);
        n /= base;
    }

    std::reverse(ret.begin(), ret.end());
    return ret;
}

bool is_palindrome(const std::string& s)
{
    int l = 0, r = s.size() - 1;
    while (l < r)
        if (s[l++] != s[r--]) return false;
    return true;
}

int main(int, char**)
{
    int base; task_in >> base;
    for (int n = 1; n <= max; ++n) {
        const auto sq = convert(n * n, base);
        if (is_palindrome(sq))
            task_out << convert(n, base) << " " << sq << "\n";
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 palsquare.cpp -o palsquare.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 palsquare.cpp -o palsquare.o

Input:

14

Output:

1 1
2 4
3 9
11 121
1A 2D2
22 484
35 B3B
BB 9CC9
101 10201
111 12321
121 14641
131 16B61

*/

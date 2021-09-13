/*
ID: marcin.8
LANG: C++
TASK: dualpal
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=O1jcIMsd1hy&S=dualpal
*/

#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>

constexpr char digits[21] = "0123456789ABCDEFGHIJ";

std::ifstream task_in("dualpal.in");
std::ofstream task_out("dualpal.out");

static std::string base_convert(int n, const int base)
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

static bool is_palindrome(const std::string& s)
{
    int l = 0, r = s.size() - 1;
    while (l < r)
        if (s[l++] != s[r--]) return false;
    return true;
}

int main(int, char**)
{
    int N, S; task_in >> N >> S;
    ++S;
    while (N > 0) {
        int found{0};
        for (int b = 2; b <= 10; ++b) {
            if (is_palindrome(base_convert(S, b)))
                ++found;
            if (found == 2) {
                --N;
                task_out << S << '\n';
                break;
            }
        }
        ++S;
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 dualpal.cpp -o dualpal.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 dualpal.cpp -o dualpal.o

Input:

3 25

Output:

26
27
28

*/

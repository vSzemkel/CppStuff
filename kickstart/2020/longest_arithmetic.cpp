
#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>

// longest_arithmetic
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ff47/00000000003bf4ed

size_t g_size;
std::vector<int64_t> g_input;

template <typename T> std::vector<T> fill(const size_t size){ std::vector<T> cont(size); std::copy_n(std::istream_iterator<T>{std::cin}, size, cont.begin()); return cont; };

int solve() {
    int ret{0}, ca{0}, seek = std::numeric_limits<int>::max();
    for (size_t i = 1; i < g_size; ++i) {
        const auto diff = g_input[i] - g_input[i - 1];
        if (diff != seek) {
            ret = std::max(ret, ca);
            seek = diff;
            ca = 1;
        } else
            ++ca;
    }

    return 1 + std::max(ret, ca);
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
        std::cin >> g_size;
        g_input = fill<int64_t>(g_size);
        std::cout << "Case #" << g << ": " << solve() << "\n";
    }
}

/*
clang++.exe -Wall -ggdb3 -O0 -std=c++17 longest_arithmetic.cpp -o longest_arithmetic.exe
g++ -Wall -ggdb3 -O0 -std=c++14 longest_arithmetic.cpp -o longest_arithmetic.o
longest_arithmetic.exe < longest_arithmetic.in

Input:

4
7
10 7 4 6 8 10 11
4
9 7 5 3
9
5 5 4 5 5 5 4 5 6
10
5 4 3 2 1 2 3 4 5 6

Output:

Case #1: 4
Case #2: 4
Case #3: 3
Case #4: 6

*/

/*
ID: marcin.8
LANG: C++
TASK: prime3
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=p77QAdyHaX0&S=prime3
*/

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <filesystem>
#include <fstream>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <vector>

std::ifstream task_in("prime3.in");
std::ofstream task_out("prime3.out");

using board_t = std::array<std::string, 5>;

static std::array<std::vector<std::pair<int, std::string>>, 10> generate(const int64_t lower, const int64_t upper, const int sum) { // generate primes in [range)
    std::vector<bool> taken(upper);
    const auto sqrt = (int)std::sqrt(upper);
    for (int i = 2; i <= sqrt; ++i)
        if (!taken[i])
            for (int j = i * i; j < upper; j += i)
                taken[j] = true;

    std::array<std::vector<std::pair<int, std::string>>, 10>primes;
    for (int i = 2; i < upper; ++i)
        if (!taken[i] && lower <= i) {
            int s{}, c{i};
            while (10 <= c) {
                s += (c % 10);
                c /= 10;
            }
            if (c + s == sum)
                primes[c].emplace_back(i, std::to_string(i));
        }

    return primes;
}

/**
 * Observation: Top row number must not contain 0 digit
 */
int main(int, char**)
{
    int S, N;
    task_in >> S >> N;

    const auto primes = generate(10000, 100000, S);
    for (const auto& top : primes[N])
        if (top.find('0') == std::string::npos)
            for (const auto& r1 : primes[N])
                for (int c = 0; c < 5; ++c) {
                }

    task_out << 0 << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 prime3.cpp -o prime3.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address prime3.cpp -o prime3

Run:
prime3.exe && type prime3.out

Input:


Output:


*/

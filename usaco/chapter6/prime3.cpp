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

static std::pair<std::array<std::vector<int>, 10>, std::array<std::vector<std::string>, 10>> generate(const int64_t lower, const int64_t upper, const int sum) { // generate primes in [range)
    std::vector<bool> taken(upper);
    const auto sqrt = (int)std::sqrt(upper);
    for (int i = 2; i <= sqrt; ++i)
        if (!taken[i])
            for (int j = i * i; j < upper; j += i)
                taken[j] = true;

    std::array<std::vector<int>, 10> iprimes;
    std::array<std::vector<std::string>, 10> sprimes;
    for (int i = 2; i < upper; ++i)
        if (!taken[i] && lower <= i) {
            int s{}, c{i};
            while (10 <= c) {
                s += (c % 10);
                c /= 10;
            }
            if (c + s == sum) {
                iprimes[c].push_back(i);
                sprimes[c].push_back(std::to_string(i));
            }
        }

    return {iprimes, sprimes};
}

/**
 * Observation: Top row number must not contain 0 digit
 */
int main(int, char**)
{
    int S, N;
    task_in >> S >> N;

    const auto [iprimes, sprimes] = generate(10000, 100000, S);
    const auto check = [&](const std::string& s) {
        const auto& bucket = sprimes[s.front() - '0'];
        return std::binary_search(bucket.begin(), bucket.end(), s);
    };

    bool found{};
    for (const auto& top : sprimes[N])
        if (top.find('0') == std::string::npos)
            for (const auto& r1 : sprimes[N]) {
                for (const auto& left : sprimes[N]) {
                    if (left.find('0') == std::string::npos)
                        continue;
                    for (int c = 1; c < 5; ++c) {
                        // TODO
                    }
                    if (check(board[2]) && check(board[3]) && check(board[4])) {
                        std::string diag.
                        diag.resize(5);
                        diag[0] = N;
                        diag[1] = r1[1];
                        diag[2] = board[2][2];
                        diag[3] = board[3][3];
                        diag[4] = board[4][4];
                        if (check(diag)) {
                            diag[0] = board[4][0];
                            diag[1] = board[3][1];
                            diag[3] = r1[3];
                            diag[4] = top.back();
                            if (check(diag)) {
                                found = true;
                                std::cout << top << '\n' << r1 << '\n';
                                for (r = 2; r < 5; ++r)
                                    std::cout << board[r] << '\n';
                                std::cout << '\n';
                            }
                        }
                    }
                }
            }

    if (!found)
        task_out << "NONE\n";
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

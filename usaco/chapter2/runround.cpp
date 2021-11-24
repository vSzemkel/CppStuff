/*
ID: marcin.8
LANG: C++
TASK: runround
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=JcNxSzhfYW3&S=runround
*/

#include <array>
#include <assert.h>
#include <fstream>
#include <vector>

std::ifstream task_in("runround.in");
std::ofstream task_out("runround.out");

int length;
std::array<int, 10> counts;
std::array<int64_t, 19> factor10;

auto nth = [](const int n, const int pos) { return n / factor10[pos] % 10; };

/**
 * @brief Generates next interesting number not less then n
 * 
 * @param n Lower bound value
 * @param strict Specifies if answer must be strictly greter then n
 * @return int 
 */
static int next_unique(const int n, const bool strict) { // unique non-zero digits, grater then n
    auto stat = [](int z) {
        int len{0};
        while (z) {
            ++counts[z % 10];
            z /= 10;
            ++len;
        }
        return len;
    };

    int ret = n;
    length = stat(ret);
    bool found{true}, increased{false};
    std::array<bool, 10> used{};
    for (int pos = length - 1, m = factor10[pos]; pos >= 0 && found; --pos, m /= 10) { 
        const int d = increased ? 0 : nth(ret, pos);
        if (d == 0 || (counts[d] > 1 && used[d])) {
            found = false;
            for (int k = d + 1; k <= 9; ++k)
                if (!used[k]) {
                    found = true;
                    ret = (ret / m + (k - d)) * m;
                    stat(ret);
                    used[k] = true;
                    increased = true;
                    break;
                }
        } else
            used[d] = true;
    }

    if (!found || (ret == n && strict))
        return -1;

    return ret;
}

/**
 * @brief Checks if number is interesting
 * 
 * @param n Number to check
 */
static bool check_roundaround(const int n) {
    std::vector<bool> seen(length);
    const int init = length - 1;
    int pos{init};
    for (int z = length; z; --z) {
        const int d = nth(n, pos);
        if (seen[d])
            return false;
        seen[d] = true;
        pos = (10 * length + pos - d) % length;
    }

    return pos == init;
}

int main(int, char**)
{
    // precompute factor10 up to the longest number
    for (int i = 0; i < int(factor10.size()); ++i)
        factor10[i] = (i == 0) ? 1 : 10 * factor10[i - 1];

    bool strict{false};
    int N; task_in >> N; ++N;
    while (true) {
        const int ans = next_unique(N, strict);
        strict = ~ans;
        if (strict) {
            N = ans;
            if (check_roundaround(N)) {
                task_out << N << '\n';
                return 0;
            }
        } else
            ++N;
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 runround.cpp -o runround.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address runround.cpp -o runround

Input:

9415268

Output:

9425618

*/

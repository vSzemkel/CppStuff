/*
ID: marcin.8
LANG: C++
TASK: prime3
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=p77QAdyHaX0&S=prime3
*/

#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <functional>
#include <utility>
#include <vector>

std::ifstream task_in("prime3.in");
std::ofstream task_out("prime3.out");

static std::vector<int> primes, inits;
static std::array<int, 5> rows{}, cols{};
static std::vector<std::array<int, 5>> solution;

static void generate(const int64_t lower, const int64_t upper, const int sum) {
    std::vector<bool> taken(upper);
    const auto sqrt = (int)std::sqrt(upper);
    const auto no0 = [](int x) { x /= 10; for (int z = 3; z; --z, x /= 10) if (x % 10 == 0) return false; return true; };
    const auto ds = [](int x) { int ret{}; while (x) { ret += x % 10; x /= 10; } return ret; };

    for (int i = 2; i <= sqrt; ++i)
        if (!taken[i])
            for (int j = i * i; j < upper; j += i)
                taken[j] = true;

    for (int i = 2; i < upper; ++i)
        if (!taken[i] && lower <= i && ds(i) == sum) {
            if (no0(i))
                inits.push_back(i);
            primes.push_back(i);
        }
}

bool check(const int p) {
    return std::binary_search(primes.begin(), primes.end(), p);
};

void print() {
    if (solution.empty())
        task_out << "NONE\n";
    else {
        const int sz = int(solution.size());
        std::sort(solution.begin(), solution.end());
        for (int i = 0; i < sz - 1; ++i) {
            const auto& b = solution[i];
            for (int r = 0; r < 5; ++r)
                task_out << b[r] << '\n';
            task_out << '\n';
        }
        const auto& b = solution.back();
        for (int r = 0; r < 5; ++r)
            task_out << b[r] << '\n';
    }
}

/**
 * Observation: Top row number must not contain 0 digit
 * Observation: Left column number must not contain 0 digit
 * Observation: Right and bottom numbers can contain only specific digits
 */
int main(int, char**)
{
    int S, N;
    task_in >> S >> N;

    generate(10000, 100000, S);

    const std::function<void(int)> inner_solve = [&](const int n)
    {
        if (n == 10) {
            int diag = rows[0] / 10000;
            diag = 10 * diag + (rows[1] / 1000) % 10;
            diag = 10 * diag + (rows[2] / 100) % 10;
            diag = 10 * diag + (rows[3] / 10) % 10;
            if (check(10 * diag + rows[4] % 10)) {
                diag = rows[4] / 10000;
                diag = 10 * diag + (rows[3] / 1000) % 10;
                diag = 10 * diag + (rows[2] / 100) % 10;
                diag = 10 * diag + (rows[1] / 10) % 10;
                if (check(10 * diag + rows[0] % 10))
                    solution.push_back(rows);
            }
        } else {
            const bool edge = n < 2;
            const int ready = n / 2;
            const auto& cont = edge ? inits : primes;
            int scale = 1;
            if (n & 1) { // column
                int low = cols[ready];
                while (low < 10000) {
                    low *= 10;
                    scale *= 10;
                }
                const int hi = low + scale;
                for (auto it = std::lower_bound(cont.begin(), cont.end(), low); it != cont.end() && *it < hi; ++it) {
                    cols[ready] = *it;
                    const auto old_rows = rows;
                    for (int c = *it, p = 4, z = 5 - ready - 1; z; --z, --p, c /= 10)
                        rows[p] = rows[p] * 10 + c % 10;
                    inner_solve(n + 1);
                    rows = old_rows;
                }
            } else { // row
                int low = edge ? N : rows[ready];
                while (low < 10000) {
                    low *= 10;
                    scale *= 10;
                }
                const int hi = low + scale;
                for (auto it = std::lower_bound(cont.begin(), cont.end(), low); it != cont.end() && *it < hi; ++it) {
                    rows[ready] = *it;
                    const auto old_cols = cols;
                    for (int r = *it, p = 4, z = 5 - ready; z; --z, --p, r /= 10)
                        cols[p] = cols[p] * 10 + r % 10;
                    inner_solve(n + 1);
                    cols = old_cols;
                }
            }
        }
    };

    inner_solve(0);

    print();
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++20 prime3.cpp -o prime3.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++20 -fsanitize=address prime3.cpp -o prime3

Run:
prime3.exe && type prime3.out

Input:

17 7

Output:

71153
13553
15137
55331
33713

and 3 more

*/

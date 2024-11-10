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
#include <set>
#include <utility>
#include <vector>

std::ifstream task_in("prime3.in");
std::ofstream task_out("prime3.out");

static int decdiag{};
static std::vector<int> primes, inits;
static std::array<int, 5> rows{}, cols{};
static std::set<std::array<int, 5>> solution;
static std::vector<int>::const_iterator start;
static std::vector<bool> prime_prefix(100000);
static const int scales[] = {10000, 10000, 10000, 1000, 1000, 100, 100, 10, 10, 1};

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
            for (int prefix = i; prefix; prefix /= 10)
                prime_prefix[prefix] = true;
        }

    inits.push_back(1e09);
    primes.push_back(1e09);
}

void print() {
    if (solution.empty())
        task_out << "NONE\n";
    else {
        auto z = solution.size();
        for (const auto s : solution) {
            for (int r = 0; r < 5; ++r)
                task_out << s[r] << '\n';
            if (--z)
                task_out << '\n';
        }
    }
}

/** Let's find a solution adding rows and columns alternating from left and top
 * Observation: Top row number must not contain 0 digit
 * Observation: Left column number must not contain 0 digit
 * Observation: Cut early: after adding row/column prefixes of not ready cols/rows grow. Reject invalid.
 * Observation: Cut early: after adding row prefixes of decreasing diagonal grows. Reject invalid.
 * Observation: Cut early: increasing diagonal is known after adding 2 rows and 3 columns
 * Observation: Checking reversibility of increasing diagonal suffices to reverse valid board 
 * Observation: Right and bottom edges contain only digits: 1, 3, 7 and 9
 * Observation: We cant't elimitating sorting in this approach
 */
int main(int, char**)
{
    int S, N;
    task_in >> S >> N;

    generate(10000, 100000, S);

    const auto incdiag = [&](int mid_row){
        return 10 * (10 * (10 * (10 * (cols[0] % 10) + (cols[1] / 10) % 10) + (mid_row / 100) % 10) + (rows[1] / 10) % 10) + rows[0] % 10;
    };

    const auto revdiag = [&](int diag){
        int revdiag{}; for (int z = 5; z; --z, diag /= 10) revdiag = 10 * revdiag + diag % 10; return revdiag;
    };

    const std::function<void(int)> inner_solve = [&](const int n)
    {
        if (n == 8) {
            rows[4] *= 10;
            for (const auto d : {1, 2, 4, 2}) {
                rows[4] += d;
                if (prime_prefix[rows[4]]) {
                    const auto br = rows[4] % 10;
                    cols[4] = 10 * cols[4] + br;
                    if (prime_prefix[cols[4]]) {
                        decdiag = 10 * decdiag + br;
                        if (prime_prefix[decdiag]) {
                            const int diag = incdiag(rows[2]);
                            if (prime_prefix[diag])
                                solution.insert(rows);
                            if (rows != cols) {
                                if (prime_prefix[revdiag(diag)]) {
                                    std::swap(rows, cols);
                                    solution.insert(rows);
                                    std::swap(rows, cols);
                                }
                            }
                        }
                    }
                }
            }
        } else {
            const bool edge = n < 2;
            const int ready = n / 2;
            const int scale = scales[n];
            const int prefix_scale = scale / 10;
            const auto& cont = edge ? inits : primes;
            if (n & 1) { // column
                const int low = cols[ready] * scale;
                const int hi = low + scale;
                const auto prows = rows;
                const auto st = edge ? start : std::lower_bound(cont.begin(), cont.end(), low);
                for (auto it = st; *it < hi; ++it) {
                    bool prefixes_ok{true};
                    for (int c = *it, p = 4, z = 5 - ready - 1; z; --z, --p, c /= 10) {
                        const auto prefix = 10 * rows[p] + c % 10;
                        if (!prime_prefix[prefix]) {
                            prefixes_ok = false;
                            if (z == 1)
                                while (*it / prefix_scale == *(it + 1) / prefix_scale) ++it;
                            break;
                        }
                        rows[p] = prefix;
                    }

                    if (prefixes_ok) {
                        cols[ready] = *it;
                        inner_solve(n + 1);
                    }
                    rows = prows;
                }
            } else { // row
                const int low = (edge ? N : rows[ready]) * scale;
                const int hi = low + scale;
                const auto pcols = cols;
                const auto pdecdiag{decdiag};
                for (auto it = std::lower_bound(cont.begin(), cont.end(), low); *it < hi; ++it) {
                    if (n == 4) {
                        const auto diag = incdiag(*it);
                        if (!prime_prefix[diag] && !prime_prefix[revdiag(diag)]) {
                            while (*it / prefix_scale == *(it + 1) / prefix_scale) ++it;
                            continue;
                        }
                    }

                    bool prefixes_ok{true};
                    for (int r = *it, p = 4, z = 5 - ready; z; --z, --p, r /= 10) {
                        const auto prefix = 10 * cols[p] + r % 10;
                        if (!prime_prefix[prefix]) {
                            prefixes_ok = false;
                            if (z == 1)
                                while (*it / prefix_scale == *(it + 1) / prefix_scale) ++it;
                            break;
                        }
                        cols[p] = prefix;
                    }

                    if (prefixes_ok) {
                        decdiag = 10 * decdiag + cols[ready] % 10;
                        if (prime_prefix[decdiag]) {
                            if (edge)
                                start = it;
                            rows[ready] = *it;
                            inner_solve(n + 1);
                        } else
                            while (*it / prefix_scale == *(it + 1) / prefix_scale) ++it;
                    }
                    cols = pcols;
                    decdiag = pdecdiag;
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

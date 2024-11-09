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
#include <unordered_set>
#include <utility>
#include <vector>

std::ifstream task_in("prime3.in");
std::ofstream task_out("prime3.out");

static int decdiag{};
static std::unordered_set<int> seen;
static std::vector<int> primes, inits;
static std::array<int, 5> rows{}, cols{};
static std::vector<bool> prime_prefix(100000);
static std::vector<std::array<int, 5>> solution;
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

static int hash(const int diag) {
    static const int M{998244353}, BASE{53};
    int64_t h = rows[0];
    h = (h * BASE + rows[1]) % M;
    h = (h * BASE + diag) % M;
    h = (h * BASE + cols[0]) % M;
    h = (h * BASE + cols[1]);
    return h % M;
}

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

/** Let's find a solution adding rows and columns alternating from left and top
 * Observation: Top row number must not contain 0 digit
 * Observation: Left column number must not contain 0 digit
 * Observation: Cut early: increasing diagonal is known after adding 2 rows and 3 columns
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

    const std::function<void(int)> inner_solve = [&](const int n)
    {
        if (n == 9) {
            if (prime_prefix[cols[4]] && prime_prefix[10 * decdiag + cols[4] % 10]) {
                solution.push_back(rows);
                if (rows != cols) {
                    int revdiag{};
                    for (int diag = incdiag(rows[2]), z = 5; z; --z, diag /= 10)
                        revdiag = 10 * revdiag + diag % 10;
                    if (prime_prefix[revdiag]) {
                        std::swap(rows, cols);
                        seen.insert(hash(revdiag));
                        solution.push_back(rows);
                        std::swap(rows, cols);
                    }
                }
            }
        } else {
            const bool edge = n < 2;
            const int ready = n / 2;
            const int scale = scales[n];
            const auto& cont = edge ? inits : primes;
            if (n & 1) { // column
                const int low = cols[ready] * scale;
                const int hi = low + scale;
                const auto prows = rows;
                const auto pdecdiag{decdiag};
                for (auto it = std::lower_bound(cont.begin(), cont.end(), low); *it < hi; ++it) {
                    bool prefixes_ok{true};
                    for (int c = *it, p = 4, z = 5 - ready - 1; z; --z, --p, c /= 10) {
                        const auto prefix = 10 * rows[p] + c % 10;
                        if (!prime_prefix[prefix]) {
                            prefixes_ok = false;
                            break;
                        }
                        rows[p] = prefix;
                    }

                    if (prefixes_ok) {
                        decdiag = 10 * decdiag + (*it / scale) % 10;
                        if (prime_prefix[decdiag]) {
                            cols[ready] = *it;
                            inner_solve(n + 1);
                        } else 
                            while (*it / scale == *(it + 1) / scale)
                                ++it;
                    }
                    rows = prows;
                    decdiag = pdecdiag;
                }
            } else { // row
                const int low = (edge ? N : rows[ready]) * scale;
                const int hi = low + scale;
                const auto pcols = cols;
                for (auto it = std::lower_bound(cont.begin(), cont.end(), low); *it < hi; ++it) {
                    if (n == 4) {
                        const auto diag = incdiag(*it);
                        if (!prime_prefix[diag] || seen.count(hash(diag))) {
                            while (*it / 100 == *(it + 1) / 100)
                                ++it;
                            continue;
                        }
                    }

                    bool prefixes_ok{true};
                    for (int r = *it, p = 4, z = 5 - ready; z; --z, --p, r /= 10) {
                        const auto prefix = 10 * cols[p] + r % 10;
                        if (!prime_prefix[prefix]) {
                            prefixes_ok = false;
                            break;
                        }
                        cols[p] = prefix;
                    }

                    if (prefixes_ok) {
                        rows[ready] = *it;
                        inner_solve(n + 1);
                    }
                    cols = pcols;
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

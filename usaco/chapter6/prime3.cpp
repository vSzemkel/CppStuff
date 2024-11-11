/*
ID: marcin.8
LANG: C++
TASK: prime3
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=p77QAdyHaX0&S=prime3
*/

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <fstream>
#include <functional>
#include <set>
#include <utility>
#include <vector>

std::ifstream task_in("prime3.in");
std::ofstream task_out("prime3.out");

template<int N>
class positional_number_t
{
  public:
    positional_number_t(const int value = 0) : _value(value) {
        syncn2d();
    }

    positional_number_t(const std::initializer_list<int> digits) {
        assert(digits.size() <= N);
        int i = int(digits.size());
        for (const int d : digits)
            _digits[--i] = d;
        sync();
    }

    positional_number_t& operator=(const int value) {
        _value = value;
        syncn2d();
        return *this;
    }

    operator int() const {
        return _value;
    }

    int operator[](const int index) const {
        return _digits[index];
    }

    int& operator[](const int index) {
        return _digits[index];
    }

    int reverse() const {
        int ret{};
        for (const int& d : _digits)
            ret = 10 * ret + d;

        return ret;
    }

    int push_back(const int digit) {
        assert(0 <= digit && digit < 10);
        _value = 10 * _value + digit;
        syncn2d();
        return _value;
    }

    int pop_back(int count) {
        assert(0 <= count && count <= N);
        for (; count; --count)
            _value /= 10;
        syncn2d();
        return _value;
    }

    void sync() {
        _value = 0;
        for (int i = N - 1; ~i; --i)
            _value = 10 * _value + _digits[i];
    }

  private:
    void syncn2d() {
        auto tmp = _value;
        for (int& d : _digits) {
            d = tmp % 10;
            tmp /= 10;
        }
    }

    int _value{};
    int _digits[N] = {};
};

using num_t = positional_number_t<5>;

static num_t decdiag{};
static std::vector<num_t> primes, inits;
static std::array<num_t, 5> rows{}, cols{};
static std::set<std::array<num_t, 5>> solution;
static std::vector<num_t>::const_iterator start;
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
        for (const auto& s : solution) {
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

    const auto incdiag = [&](const num_t& mid_row){
        return num_t{cols[0][0], cols[1][1], mid_row[2], rows[1][1], rows[0][0]};
    };

    const std::function<void(int)> inner_solve = [&](const int n)
    {
        if (n == 8) {
            auto& lr = rows[4];
            lr.push_back(0);
            for (const auto d : {1, 2, 4, 2}) {
                lr = lr + d;
                if (prime_prefix[lr]) {
                    cols[4].push_back(lr[0]);
                    if (prime_prefix[cols[4]]) {
                        decdiag.push_back(lr[0]);
                        if (prime_prefix[decdiag]) {
                            const auto diag = incdiag(rows[2]);
                            if (prime_prefix[diag])
                                solution.insert(rows);
                            if (rows != cols) {
                                if (prime_prefix[diag.reverse()]) {
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
                    for (int p = 4, z = 5 - ready - 1; z; --z, --p) {
                        const auto prefix = int(rows[p].push_back((*it)[4 - p]));
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
                const int low = (edge ? N : int(rows[ready])) * scale;
                const int hi = low + scale;
                const auto pcols = cols;
                const auto pdecdiag{decdiag};
                for (auto it = std::lower_bound(cont.begin(), cont.end(), low); *it < hi; ++it) {
                    if (n == 4) {
                        const auto diag = incdiag(*it);
                        if (!prime_prefix[diag] && !prime_prefix[diag.reverse()]) {
                            while (*it / prefix_scale == *(it + 1) / prefix_scale) ++it;
                            continue;
                        }
                    }

                    bool prefixes_ok{true};
                    for (int p = 4, z = 5 - ready; z; --z, --p) {
                        const auto prefix = int(cols[p].push_back((*it)[4 - p]));
                        if (!prime_prefix[prefix]) {
                            prefixes_ok = false;
                            if (z == 1)
                                while (*it / prefix_scale == *(it + 1) / prefix_scale) ++it;
                            break;
                        }
                        cols[p] = prefix;
                    }

                    if (prefixes_ok) {
                        decdiag.push_back(cols[ready][0]);
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

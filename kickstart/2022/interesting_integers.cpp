
#include <algorithm>
#include <cstring>
#include <iostream>
#include <vector>

// Interesting Integers
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb33e/00000000009e73ea

constexpr const int LOG = 13;
constexpr const int SUM = 12 * 9 + 1;

int len, mod;
std::vector<int> digits;
bool    seen[LOG][SUM][SUM][2][2];
int64_t memo[LOG][SUM][SUM][2][2];

/**
 * @brief 
 * Counts interesting integers with digits of arbitrary sum
 * MSD is on position 0, LSD on len-1
 * Written by racsosabe
 * @param pos Position of the digit to consider in this run
 * @param prod Product of digits to the left of pos
 * @param suma Sum of digits to the left of pos
 * @param menor 0|1 Flag if considering numbers with digit d on position pos equal to the digit in upper bound number
 * @param start 0|1 Flag if considering numbers with any digit d on position to the left of pos greater then 0
 * @return int64_t Count of numbers with required property
 */
static int64_t dpfun(const int pos, const int prod, const int suma, const int menor, const int start) {
    if (suma > mod) 
        return 0;
    if (pos == len)
        return prod == 0 && suma == mod;
    auto& s = seen[pos][prod][suma][menor][start];
    auto& m = memo[pos][prod][suma][menor][start];
    if (s)
        return m;

    int64_t ans{0};
    const int limit = menor ? 9 : digits[pos];
    for (int i = 0; i <= limit; ++i) {
        const int nprod = start ? (prod * i) % mod : (i > 0 ? i % mod : 0);
        const int nsum = suma + i;
        const int nmenor = menor | (i < digits[pos]);
        const int nstart = start | (i > 0);
        ans += dpfun(pos + 1, nprod, nsum, nmenor, nstart);
    }

    s = true;
    return m = ans;
}

/**
 * @brief Count interesting integers in range [0..n]
 */
static int64_t _solve(int64_t n) {
    if (n == 0)
        return 0;

    len = 0;
    int64_t probe{1};
    while (probe <= n) {
        probe *= 10;
        ++len;
    }
    const int max_digits_sum = n / (probe / 10) + 9 * (len - 1);

    digits.resize(len);
    for (int z = len - 1; ~z; --z) {
        digits[z] = n % 10;
        n /= 10;
    }

    int64_t ans{0};
    for (mod = 1; mod <= max_digits_sum; ++mod) {
        memset(seen, 0, sizeof(seen));
        ans += dpfun(0, 0, 0, 0, 0);
    }

    return ans;
}

static void solve() {
    int64_t A, B;
    std::cin >> A >> B;
    std::cout << _solve(B) - _solve(A - 1);
}

static void solve_brute() { // TLE
    int64_t A, B;
    std::cin >> A >> B;

    int ret{0};
    for (auto n = A; n <= B; ++n) {
        int64_t k{n}, m{1}, s{0};
        while (k) {
            const int d = k % 10;
            m *= d;
            s += d;
            k /= 10;
        }
        if ((m % s) == 0)
            ++ret;
    }

    std::cout << ret;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 interesting_integers.cpp -o interesting_integers.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address interesting_integers.cpp -o interesting_integers

Run:
interesting_integers.exe < interesting_integers.in

Input:

4
1 9
91 99
451 460
501 1000

Output:

Case #1: 9
Case #2: 0
Case #3: 5
Case #4: 176

*/

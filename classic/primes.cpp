
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <vector>

// Primes - generation and factorization

static int is_prime(const int64_t n) {
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int64_t i = 5; i * i <= n; i += 2)
        if (n % i == 0)
            return false;
    return true;
}

static std::vector<int64_t> generate(const int64_t n) { // generate all less then n
    std::vector<bool> taken(n);
    const auto sqrt = (int64_t)std::sqrt(n);
    for (int i = 2; i <= sqrt; ++i)
        if (!taken[i])
            for (int64_t j = i * i; j < n; j += i)
                taken[j] = true;

    std::vector<int64_t> primes;
    for (int i = 2; i < n; ++i)
        if (!taken[i]) primes.push_back(i);

    return primes;
}

static std::vector<int64_t> factorize(int64_t n) {
    std::vector<int64_t> factorization;
    if (n < 1) return factorization;

    for (int d : {2, 3, 5}) {
        while (n % d == 0) {
            factorization.push_back(d);
            n /= d;
        }
    }

    int i{0}, increments[] = {6, 4, 2, 4, 2, 4, 6, 2};
    for (int64_t d = 7; d * d <= n; d += increments[i]) {
        while (n % d == 0) {
            factorization.push_back(d);
            n /= d;
        }
        i = (i + 1) % 8;
    }

    if (n > 1 || factorization.empty())
        factorization.push_back(n);

    return factorization;
}

static auto get_all_divisors(const int64_t n) {
    std::vector<int64_t> ans;
    int c{1};
    for (; c * c < n; ++c) {
        const auto d = n / c;
        if (c * d == n) {
            ans.push_back(c);
            ans.push_back(d);
        }
    }
    if (c * c == n)
        ans.push_back(c);

    return ans;
}

// https://cp-algorithms.com/algebra/divisors.html
static auto get_base_divisors(const int64_t n) {
    const auto factorization = factorize(n);
    assert(std::is_sorted(factorization.begin(), factorization.end()));
    std::unordered_map<int64_t, int> divisors;
    for (const auto d : factorization)
        ++divisors[d];
    return divisors;
}

static int64_t count_divisors(const int64_t n) {
    if (n < 2)
        return 1;
    int64_t ret{1};
    const auto divisors = get_base_divisors(n);
    for (const auto& [_, c] : divisors)
        ret *= (c + 1);
    return ret;
}

static int64_t sum_divisors(const int64_t n) {
    int64_t ret{1};
    const auto divisors = get_base_divisors(n);
    for (const auto& [d, c] : divisors)
        ret *= (std::pow(d, c + 1) - 1) / (d - 1);
    return ret;
}

int main(int, char**)
{
    const int p = 998244353;
    assert(is_prime(p));
    auto primes = generate(16); // square case
    assert(primes.size() == 6);
    primes = generate(500);
    assert(primes.size() == 95);
    primes = generate(1e09); // real 0m7.684s
    assert(primes.size() == 50847534);


    const int64_t n = 2LL * 7 * 7 * 13 * 37 * 107 * 499 * 1039 * 7013;
    assert(factorize(0).empty());
    assert(factorize(1) == (std::vector<int64_t>{1}));
    assert(factorize(p) == (std::vector<int64_t>{p}));
    assert(factorize(n) == (std::vector<int64_t>{2, 7, 7, 13, 37, 107, 499, 1039, 7013}));
    assert(count_divisors(n) == 384); // 3 * 2**7
    assert(sum_divisors(2 * 2 * 5 * 7) == 336); // 1 + 2 + 5 + 7 + 4 + 10 + 14 + 35 + 140/2 + 140/5 + 140/7 + 140
    const auto d888888 = get_all_divisors(888888);
    assert(count_divisors(888888) == int64_t(d888888.size()));
    assert(sum_divisors(888888) == std::accumulate(d888888.begin(), d888888.end(), 0LL));
    std::cout << "PASSED\n";
}

/* Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 primes.cpp -o primes.exe
*/

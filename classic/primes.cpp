
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <vector>

// Primes - generation, primarily test and factorization

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

// Deterministic Miller-Rabin primality test for all uint64_t values.
static bool is_prime(const uint64_t n) {
    constexpr auto modular_power = [](uint64_t base, uint64_t exponent, const uint64_t modulus) {
        uint64_t result{1};
        base %= modulus;
        while (exponent > 0) {
            if (exponent & 1)
                result = static_cast<__uint128_t>(result) * base % modulus;
            base = static_cast<__uint128_t>(base) * base % modulus;
            exponent >>= 1;
        }
        return result;
    };

    constexpr auto is_prime_naive = [](const uint64_t n) {
        if (n == 2 || n == 3) return true;
        if (n < 2 || n % 2 == 0 || n % 3 == 0) return false;
        for (uint64_t i = 5; i * i <= n; i += 2)
            if (n % i == 0)
                return false;
        return true;
    };

    if (n < 100)
        return is_prime_naive(n);

    uint64_t d = n - 1;
    int s{0};
    while (d % 2 == 0) {
        d /= 2;
        ++s;
    }

    for (const uint64_t base : {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL}) {
        if (base % n == 0)
            continue;

        uint64_t x = modular_power(base, d, n);
        if (x == 1 || x == n - 1)
            continue;

        bool probably_prime{false};
        for (int r = 1; r < s; ++r) {
            x = static_cast<__uint128_t>(x) * x % n;
            if (x == n - 1) {
                probably_prime = true;
                break;
            }
        }
        if (!probably_prime)
            return false;
    }

    return true;
}

static std::vector<uint64_t> factorize(uint64_t n) {
    std::vector<uint64_t> factorization;
    if (n < 1) return factorization;

    for (int d : {2, 3, 5}) {
        while (n % d == 0) {
            factorization.push_back(d);
            n /= d;
        }
    }

    int i{0}, increments[] = {6, 4, 2, 4, 2, 4, 6, 2};
    for (uint64_t d = 7; d * d <= n; d += increments[i]) {
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

static auto get_all_divisors(const uint64_t n) {
    std::vector<uint64_t> ans;
    uint64_t c{1};
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
static auto get_base_divisors(const uint64_t n) {
    const auto factorization = factorize(n);
    assert(std::is_sorted(factorization.begin(), factorization.end()));
    std::unordered_map<uint64_t, int> divisors;
    for (const auto d : factorization)
        ++divisors[d];
    return divisors;
}

static int count_divisors(const uint64_t n) {
    if (n < 2)
        return 1;
    int ret{1};
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
    auto primes = generate(16); // square case
    assert(primes.size() == 6);
    primes = generate(500);
    assert(primes.size() == 95);
    primes = generate(1e09); // real 0m7.684s
    assert(primes.size() == 50847534);

    const int p = 998244353;
    assert(is_prime(p));
    assert(!is_prime(0));
    assert(!is_prime(1));
    assert(is_prime(2));
    assert(is_prime(3));
    assert(!is_prime(561));
    assert(!is_prime(341550071728321ULL));
    assert(is_prime(18446744073709551557ULL));

    const uint64_t n = 2ULL * 7 * 7 * 13 * 37 * 107 * 499 * 1039 * 7013;
    assert(factorize(0).empty());
    assert(factorize(1) == (std::vector<uint64_t>{1}));
    assert(factorize(p) == (std::vector<uint64_t>{p}));
    assert(factorize(n) == (std::vector<uint64_t>{2, 7, 7, 13, 37, 107, 499, 1039, 7013}));
    assert(count_divisors(n) == 384); // 2 * 3 * 2 * 2 * 2 * 2 * 2 * 2
    assert(sum_divisors(2 * 2 * 5 * 7) == 336); // 1 + 2 + 5 + 7 + 4 + 10 + 14 + 35 + 140/2 + 140/5 + 140/7 + 140
    const auto d888888 = get_all_divisors(888888);
    assert(count_divisors(888888) == int(d888888.size()));
    assert(sum_divisors(888888) == std::accumulate(d888888.begin(), d888888.end(), 0LL));
    std::cout << "PASSED\n";
}

/* Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 --rtlib=compiler-rt primes.cpp
*/

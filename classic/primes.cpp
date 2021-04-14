
#include <assert.h>
#include <cmath>
#include <vector>

// Primes - generation and factorization

static std::vector<int64_t> generate(const int64_t n) {
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

    if (n > 1)
        factorization.push_back(n);

    return factorization;
}

int main(int, char**)
{
    auto primes = generate(500);
    assert(primes.size() == 95);

    const int64_t n = 2LL * 7 * 7 * 13 * 37 * 107 * 499 * 1039 * 7013;
    assert(factorize(n) == (std::vector<int64_t>{2, 7, 7, 13, 37, 107, 499, 1039, 7013}));
}

/* Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 primes.cpp -o primes.exe
*/

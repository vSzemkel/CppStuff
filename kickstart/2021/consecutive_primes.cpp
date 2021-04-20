
#include <cmath>
#include <iostream>
#include <vector>

// Consecutive primes
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000435a5b/000000000077a8e6

int64_t g_primes_bound{0};
std::vector<int64_t> g_primes;

static bool is_prime(int64_t n) {
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

    return factorization.size() == 1;
}

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

static void solve() {
    int64_t Z; std::cin >> Z;
    const int64_t SQRT = std::ceil(std::sqrt(Z));
    auto gt_or_eq = SQRT;
    while (!is_prime(gt_or_eq))
        ++gt_or_eq;

    auto lt1 = SQRT - 1;
    while (!is_prime(lt1))
        --lt1;

    auto candidate = gt_or_eq * lt1;
    if (candidate > Z) {
        auto lt2 = lt1 - 1;
        while (!is_prime(lt2))
            --lt2;

        candidate = lt1 * lt2;
    }

    std::cout << candidate;
}

static void solve_set2() {
    int64_t Z; std::cin >> Z;
    int64_t N = std::max(10., std::sqrt(Z) + 300.);

    if (g_primes_bound < N)
        g_primes = generate(N);
    int size = (int)g_primes.size();
    while (g_primes[size - 1] * g_primes[size - 2] > Z)
        --size;

    std::cout << g_primes[size - 1] * g_primes[size - 2];
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 consecutive_primes.cpp -o consecutive_primes.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address consecutive_primes.cpp -o consecutive_primes.o

Run:
consecutive_primes.exe < consecutive_primes.in

Input:

4
6
20
2021
2020

Output:

Case #1: 6
Case #2: 15
Case #3: 2021
Case #4: 1763

*/

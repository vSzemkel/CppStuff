
#include <algorithm>
#include <cmath>
#include <iostream>
#include <format>
#include <utility>
#include <vector>

// Prime Subtractorization
// https://www.facebook.com/codingcompetitions/hacker-cup/2024/round-1/problems/B

std::vector<std::pair<int, int>> primes; // {prime, number of primes <= prime, having prime smaller by 2}

static void precompute(const int64_t n)
{
    primes.emplace_back(1, 1);
    std::vector<bool> taken(n);
    const auto sqrt = (int64_t)std::sqrt(n);
    for (int i = 2; i <= sqrt; ++i)
        if (!taken[i])
            for (int64_t j = i * i; j < n; j += i)
                taken[j] = true;

    for (int i = 2; i < n; ++i)
        if (!taken[i]) {
            const auto& [p, c] = primes.back();
            primes.emplace_back(i, p + 2 == i ? c + 1 : c);
        }
}

static void solve()
{
    int N;
    std::cin >> N;
    if (N < 5) {
        std::cout << 0;
        return;
    }

    auto it = std::lower_bound(primes.begin(), primes.end(), std::pair<int, int>{N, 0});
    if (it->first != N)
        --it;
    std::cout << it->second;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    precompute(10'000'020);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << std::format("Case #{}: ", g);
        solve();
        std::cout << '\n';
    }
}

/*

Format:
clang-format -i prime_subtractorization.cpp

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 prime_subtractorization.cpp -o prime_subtractorization.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address prime_subtractorization.cpp -o prime_subtractorization

Run:
prime_subtractorization.exe < prime_subtractorization.in

Input:


Output:


*/

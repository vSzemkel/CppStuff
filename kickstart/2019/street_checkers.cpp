
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Street Checkers
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050edb/00000000001707b9

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

std::unordered_map<int, int> divisor_count;

static void solve() {
    int L, R;
    std::cin >> L >> R;

    int ans{0};
    for (int x = L; x <= R; ++x) {
        int totalDiv, oddDiv;
        if (divisor_count.count(x))
            totalDiv = divisor_count[x];
        else
            totalDiv = divisor_count[x] = count_divisors(x);

        int e{x};
        while ((e & 1) == 0) e >>= 1;
        if (divisor_count.count(e))
            oddDiv = divisor_count[e];
        else
            oddDiv = divisor_count[e] = count_divisors(e);

        if (std::abs(totalDiv - 2 * oddDiv) < 3)
            ++ans;
    }

    std::cout << ans;
}

std::unordered_set<int> good, bad;

static void solve_naive() {
    int L, R;
    std::cin >> L >> R;

    int ans{0};
    std::vector<bool> painted;
    for (int x = L; x <= R; ++x) {
        if (good.count(x)) {
            ++ans;
            continue;
        }
        if (bad.count(x))
            continue;

        painted.clear();
        int c{1};
        for (; c * c < x; ++c) {
            const auto d = x / c;
            if (c * d == x) {
                painted.push_back(c % 2);
                painted.push_back(d % 2);
            }
        }
        if (c * c == x)
            painted.push_back(c % 2);

        const auto popcnt = std::count_if(painted.begin(), painted.end(), [](const auto b){ return b; });
        if (std::abs(int(painted.size() - 2 * popcnt)) < 3) {
            good.insert(x);
            ++ans;
        } else
            bad.insert(x);
    }

    std::cout << ans;
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 street_checkers.cpp -o street_checkers.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address street_checkers.cpp -o street_checkers

Run:
street_checkers.exe < street_checkers.in

Input:

2
5 10
102 102
1974 2075

Output:

Case #1: 5
Case #2: 1
Case #3: 43

*/

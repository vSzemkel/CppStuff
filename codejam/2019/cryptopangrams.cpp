
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

// Cryptopangrams
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000051705/000000000008830b#problem

static void solve() {
    int N, L; std::cin >> N >> L;
    std::vector<int64_t> tokens(L);
    for (int i = 0; i < L; ++i)
        std::cin >> tokens[i];

    assert(L > 1);
    std::vector<int64_t> sentence(L + 1);
    int i{0};
    for (; i < L - 1; ++i) {
        const auto gcd = std::gcd(tokens[i], tokens[i + 1]);
        if (gcd > 1 && gcd < tokens[i]) { // found a vanila prime factor
            sentence[i + 1] = gcd;
            break;
        }
    }

    assert(i < L - 1);
    for (int j = i + 2; j < L + 1; ++j)
        sentence[j] = tokens[j - 1] / sentence[j - 1];
    for (int j = i; j >= 0; --j)
        sentence[j] = tokens[j] / sentence[j + 1];

    auto primes = sentence;
    std::sort(primes.begin(), primes.end());
    const auto enduni = std::unique(primes.begin(), primes.end());
    primes.erase(enduni, primes.end());
    assert(primes.size() == 26);

    std::unordered_map<int64_t, char> dict;
    for (int i = 0; i < (int)primes.size(); ++i)
        dict[primes[i]] = 'A' + i;

    for (const auto& t : sentence)
        std::cout << dict[t];
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 cryptopangrams.cpp -o cryptopangrams.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 cryptopangrams.cpp -o cryptopangrams.o

Run:
cryptopangrams.exe < cryptopangrams.in

Input:

2
103 31
217 1891 4819 2291 2987 3811 1739 2491 4717 445 65 1079 8383 5353 901 187 649 1003 697 3239 7663 291 123 779 1007 3551 1943 2117 1679 989 3053
10000 25
3292937 175597 18779 50429 375469 1651121 2102 3722 2376497 611683 489059 2328901 3150061 829981 421301 76409 38477 291931 730241 959821 1664197 3057407 4267589 4729181 5335543

Output:

Case #1: AABBAAACJQUIZKNOWBEVYOFDPFLUXALGORITHMS
Case #2: CJQUIZKNOWBEVYOFDPFLUXALGORITHMS

*/

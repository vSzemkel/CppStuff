#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

// Binominal coefficient
// For binominal coefficient modulo M, see ./modnum.cpp

constexpr int MAX = 10000;
double logFactorials[MAX];

double factorial(const int n) {
    return std::tgamma(n + 1);
}

double donotuse(const int n, const int k) {
    return factorial(n) / factorial(n - k) / factorial(k);
}

double brute(const int n, const int k) { // for n < 67 use pascal
    double ret{1};
    for (int i = 0; i < k; ++i)
        ret = (ret * (n - i)) / (i + 1);
    return ret;
}

double bin_coeff(const int n, const int k) {
    return 1 / ((n + 1) * std::beta(n - k + 1, k + 1));
}

// multiply the result with 2**n to get bin_coeff(n, k)
double choose(const int n, const int k) {
    if (k < 0 || k > n) return 0;
    return std::pow(2, logFactorials[n] - logFactorials[k] - logFactorials[n - k] - n);
}

static auto pascal(const int n) {
    assert(n < 67);
    std::vector<std::vector<int64_t>> ret(n + 1, std::vector<int64_t>(n + 1));
    for (int r = 0; r <= n; ++r) {
        ret[r][0] = ret[r][r] = 1;
        for (int c = 1; c < r; ++c)
            ret[r][c] = ret[r - 1][c - 1] + ret[r - 1][c];
    }

    return ret;
}

static auto pascal_sparse(const int n) {
    std::vector<std::vector<int64_t>> ret(n + 1, std::vector<int64_t>(n + 1, -1));
    for (int r = 0; r <= n; ++r) {
        ret[r][0] = ret[r][r] = 1;
        for (int c = 1; c < r; ++c) {
            const auto left = ret[r - 1][c - 1];
            const auto right = ret[r - 1][c];
            if (~left && ~right) {
                const auto sum = left + right;
                if (sum > 0)
                    ret[r][c] = sum;
            }
        }
    }

    return ret;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    auto b = brute(137, 8);
    auto c = bin_coeff(137, 8);
    assert(std::abs(b - c) < 1);
    std::cout << std::floor(std::log10(c)) + 1 << std::endl;

    b = brute(197, 8);
    c = bin_coeff(197, 8);
    std::cout << std::floor(std::log10(c)) + 1 << std::endl;
    assert(b != c);
    assert(std::abs(b - c) < 1);

    logFactorials[0] = 0.0;
    for (int i = 1; i < MAX; i++)
        logFactorials[i] = logFactorials[i-1] + std::log2(i);

    const auto ps = pascal_sparse(200);
    auto a = ps[197][12];
    b = brute(197, 12);
    c = bin_coeff(197, 12);
    auto d = std::pow(2, 197) * choose(197, 12);
    std::cout << std::floor(std::log10(c)) + 1 << std::endl;
    assert(std::abs(a - b) < 1);
    assert(std::abs((a - d) / d) < 0.00000000001);

    const auto p = pascal(66);
    const auto pa = p[66][32];
    const auto br = brute(66, 32);
    assert(std::abs((pa - br) / br) < 0.00000000001);
    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 binominal_coefficient.cpp -o binominal_coefficient.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address binominal_coefficient.cpp -o binominal_coefficient.o

*/

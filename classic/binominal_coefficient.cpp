#include <cassert>
#include <cmath>
#include <iostream>

// Binominal coefficient

constexpr int MAX = 10000;
double logFactorials[MAX];

double factorial(const int n) {
    return std::tgamma(n + 1);
}

double donotuse(const int n, const int k) {
    return factorial(n) / factorial(n - k) / factorial(k);
}

double brute(const int n, const int k) {
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

    auto a = brute(197, 8);
    b = bin_coeff(197, 8);
    c = std::pow(2, 197) * choose(197, 8);
    std::cout << std::floor(std::log10(c)) + 1 << std::endl;
    assert(std::abs(a - b) < 1);
    assert(std::abs((b - c) / c) < 0.00000000001);
    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 binominal_coefficient.cpp -o binominal_coefficient.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address binominal_coefficient.cpp -o binominal_coefficient.o

*/

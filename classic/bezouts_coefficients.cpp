#include <array>
#include <cassert>
#include <iostream>

// Bezout's coefficients

template <typename T = int>
static auto bezouts_coefficients(const T a, const T b) {
    if (b == 0)
        return std::array<T, 2>{1, 0};
    const auto c = bezouts_coefficients(b, a % b);
    return std::array<T, 2>{c[1], c[0] - (a / b) * c[1]};
}

constexpr const int64_t prime1 = 15485747;
constexpr const int64_t prime2 = 11998487;
constexpr const int64_t prime3 = 952559;

int main(int, char**)
{
    // gcd(p1*p3, p2*p3) == p3
    const auto bc = bezouts_coefficients<int64_t>(prime1 * prime3, prime2 * prime3);
    assert(prime1 * prime3 * bc[0] + prime2 * prime3 * bc[1] == prime3);
    std::cout << "coeff0 = " << bc[0] << ", coeff1 = " << bc[1] << '\n';
    std::cout << "PASSED\n";
}

/* Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 bezouts_coefficients.cpp -o bezouts_coefficients.exe
*/

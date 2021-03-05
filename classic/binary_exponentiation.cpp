
#include <iostream>

// Binary exponentiation
// Compute A**N % M in O(log N)

int main(int, char**)
{
    uint64_t A = 2784892;
    uint64_t N = 7391;
    const uint64_t M = 998244353;
    std::cout << "pow(" << A << ", " << N << ", " << M << ") = ";

    A %= M;
    int ret{1};
    while (N > 0) {
        if (N & 1)
            ret = (ret * A) % M;
        A = A * A % M;
        N >>= 1;
    }

    std::cout << ret << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 binary_exponentiation.cpp -o binary_exponentiation.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 binary_exponentiation.cpp -o binary_exponentiation.o

*/

#include <iostream>

int main(int, char**) {
    int i{};
    float f{};
    while (i == static_cast<decltype(i)>(f))
        f = ++i;
    std::cout << "In this implementation of float significant has " << std::log2(i - 1) << " bits.\n";
    std::cout << "First non exact value: " << i << ' ' << int(f) << '\n';

    int64_t ll = (1LL << 53) - 100;
    double d = static_cast<double>(ll);
    while (ll ==  static_cast<decltype(ll)>(d))
        d = ++ll;
    std::cout << "In this implementation of double significant has " << std::log2(ll - 1) << " bits.\n";
    std::cout << "First non exact value: " << ll << ' ' << int64_t(d) << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 exact_floats.cpp -o exact_floats.exe

*/

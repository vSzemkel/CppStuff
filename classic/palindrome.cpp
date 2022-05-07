
#include <assert.h>
#include <iostream>
#include <string>

// see generating palindromes: /usaco/pprime

bool is_palindrome(const std::string& s)
{
    int l = 0, r = s.size() - 1;
    while (l < r)
        if (s[l++] != s[r--]) return false;
    return true;
}

template <typename T>
static int numlen(const T n) {
    int len{0}; 
    T probe{1};
    while (probe < n) {
        probe *= 10;
        ++len;
    }
    return len;
}

template <typename T>
static bool is_num_palindrome(T n) {
    if (n < 10) return true;
    int len = numlen(n);

    bool odd = len & 1;
    if (!odd && (n % 11) != 0)
        return false;

    len /= 2;
    T m{0};
    for (int z = len; z; --z) {
        m *= 10;
        m += n % 10;
        n /= 10;
    }

    if (odd)
        n /= 10;

    return m == n;
}

template <typename T>
static bool is_bit_palindrome(const T v, const int size) {
    T l = 1, u = 1 << (size - 1);
    while (l < u) {
        if (bool(v & l) != bool(v & u)) return false;
        l <<= 1;
        u >>= 1;
    }
    return true;
}

template <typename T>
static T numrev(T n) {
    T m{0};
    while (n) {
        m *= 10;
        m += n % 10;
        n /= 10;
    }
    return m;
}

int main(int, char**)
{
    assert(numlen(8'300'986'714'298LL) == 13);
    assert(numrev(907'856'452'311LL) == 113'254'658'709LL);
    assert(is_bit_palindrome(21845, 15) && !is_bit_palindrome(21846, 15));
    for (int64_t i = 0; i < 50'000'000; ++i)
        if (is_num_palindrome(i) != is_palindrome(std::to_string(i)))
            assert(false);
    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 palindrome.cpp -o palindrome.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address palindrome.cpp -o palindrome

*/

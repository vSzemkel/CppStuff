
#include <assert.h>
#include <string>

bool is_palindrome(const std::string& s)
{
    int l = 0, r = s.size() - 1;
    while (l < r)
        if (s[l++] != s[r--]) return false;
    return true;
}

static int numlen(const int64_t n) {
    int len{0}; 
    int64_t probe{1};
    while (probe < n) {
        probe *= 10;
        ++len;
    }
    return len;
}

static bool is_num_palindrome(int64_t n) {
    if (n < 10) return true;
    int len = numlen(n);

    int64_t m{0};
    bool odd = len & 1;
    len /= 2;
    for (int z = len; z; --z) {
        m *= 10;
        m += n % 10;
        n /= 10;
    }

    if (odd)
        n /= 10;

    return m == n;
}

int main(int, char**)
{
    for (int64_t i = 0; i < 10'000'000; ++i)
        if (is_num_palindrome(i) != is_palindrome(std::to_string(i)))
            assert(false);
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 palindrome.cpp -o palindrome.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address palindrome.cpp -o palindrome

*/

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// Simple operations on digits


constexpr char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
constexpr int bases_count = (int)sizeof(digits) - 1;
std::unordered_map<char, int> digit_value;
std::vector<int64_t> pow10(19);

static std::string from_base10(int64_t n, const int base)
{
    assert(1 < base && base < bases_count);

    std::string ret;
    while (n > 0) {
        ret.push_back(digits[n % base]);
        n /= base;
    }

    std::reverse(ret.begin(), ret.end());
    return ret;
}

static int64_t to_base10(std::string num, int base)
{
    assert(1 < base && base < bases_count);

    int64_t ret{0}, factor{1};
    while (!num.empty()) {
        ret += factor * digit_value[num.back()];
        factor *= base;
        num.pop_back();
    }

    return ret;
}

static int count_digits(int64_t n)
{
    int d{1};
    while (true) {
      n /= 10;
      if (n == 0) break;
      ++d;
    }

    return d;
}

static int64_t head_digits(const int64_t n, const int count)
{
    const int all = count_digits(n);
    assert(0 <= count && count <= all);
    return n / pow10[all - count];

}

static int64_t tail_digits(const int64_t n, const int count)
{
    const int all = count_digits(n);
    assert(0 <= count && count <= all);
    return n % pow10[count];

}

static void init()
{
    int64_t p{1};
    for (auto& pow : pow10) {
        pow = p;
        p *= 10;
    }

    for (int b = 0; b < bases_count; ++b)
        digit_value[digits[b]] = b;
}

int main(int, char**)
{
    init();

    for (int n = 1; n <= 100001; ++n) {
        const int dc = count_digits(n);
        for (int i = 0; i <= dc; ++i)
            assert(head_digits(n, dc - i) * pow10[i] + tail_digits(n, i) == n);
        for (int b = 2; b < bases_count; ++b)
            assert(to_base10(from_base10(n, b), b) == n);
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 digits.cpp -o digits.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address digits.cpp -o digits.o

*/

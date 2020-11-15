
#include <cmath>
#include <iostream>
#include <string>

// Boring numbers
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ff49/000000000043b0c6

int64_t g_a, g_b;
const std::string g_min_boring = "10101010101010101010";
const std::string g_max_boring = "98989898989898989898";

bool is_boring(const int64_t n)
{
    bool ret{true};
    auto s = std::to_string(n);
    for (int i = 0; ret && i < s.size(); ++i)
        ret &= ((i + 1) % 2) == ((s[i] - '0') % 2);
    return ret;
}

int64_t boring_ceil(int64_t n)
{
    const int digits = 1 + (int)log10(n);
    for (int pos = digits; pos > 0; --pos) {
        const int64_t p10 = pow(10, pos - 1);
        const int d = (n / p10) % 10;
        if (((digits - pos) % 2) == (d % 2)) {
            n = ((n + p10) / p10) * p10;
            n += std::atoll(g_min_boring.substr(digits - pos + 1, pos - 1).c_str());
            break;
        }
    }

    if (1 + (int)log10(n) > digits)
        n = std::atoll(g_min_boring.substr(0, digits + 1).c_str());

    return n;
}

int64_t boring_floor(int64_t n)
{
    const int digits = 1 + (int)log10(n);
    for (int pos = digits; pos > 0; --pos) {
        const int64_t p10 = pow(10, pos - 1);
        const int d = (n / p10) % 10;
        if (((digits - pos) % 2) == (d % 2)) {
            n = ((n - p10) / p10) * p10;
            n += std::atoll(g_max_boring.substr(digits - pos + 1, pos - 1).c_str());
            break;
        }
    }

    if (1 + (int)log10(n) < digits)
        n = std::atoll(g_max_boring.substr(0, digits - 1).c_str());

    return n;
}

int64_t smart()
{
    const auto boring_ordinal = [](int64_t n){
        int64_t ret{0}, factor{1};
        while (n > 0) {
            const auto d = n % 10;
            ret += (d >> 1) * factor;
            factor *= 5;
            n /= 10;
        }
        return ret;
    };

    int64_t ret{0};
    int64_t a = boring_ceil(g_a);
    int64_t b = boring_floor(g_b);
    const int digits_a = 1 + log10(a);
    const int digits_b = 1 + log10(b);
    for (int i = digits_a; i < digits_b; ++i) {
        b = std::atoll(g_max_boring.substr(0, i).c_str());
        if (b >= a)
            ret += boring_ordinal(b - a) + 1;
        a = std::atoll(g_min_boring.substr(0, i + 1).c_str());
    }

    b = boring_floor(g_b);
    b -= a;
    if (b >= 0)
        ret += boring_ordinal(b) + 1;

    return ret;
}

int64_t brute() {
    int64_t ret{0};
    for (auto i = g_a; i <= g_b; ++i)
        if (is_boring(i))
            ++ret;

    return ret;
}

int64_t dull_subtask(int64_t n)
{
    const int last = n % 10;
    int64_t ret = (g_a <= n && n <= g_b) ? 1 : 0;

    n *= 10;
    if (n <= g_b) {
        if (last % 2 == 0) {
            ret += dull_subtask(n + 1);
            ret += dull_subtask(n + 3);
            ret += dull_subtask(n + 5);
            ret += dull_subtask(n + 7);
            ret += dull_subtask(n + 9);
        } else {
            ret += dull_subtask(n + 2);
            ret += dull_subtask(n + 4);
            ret += dull_subtask(n + 6);
            ret += dull_subtask(n + 8);
            ret += dull_subtask(n + 0);
        }
    }
    
    return ret;
}

int64_t dull() {
    int64_t ret{0};
    ret += dull_subtask(1);
    ret += dull_subtask(3);
    ret += dull_subtask(5);
    ret += dull_subtask(7);
    ret += dull_subtask(9);

    return ret;
}

int main(int argc, char* argv[])
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    // parse console input
    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cin >> g_a >> g_b;
        std::cout << "Case #" << g << ": " << smart() << "\n";
        std::cout << "Case #" << g << ": " << brute() << "\n";
    }
}

/*
clang++.exe -Wall -ggdb3 -O0 -std=c++17 boring_numbers.cpp -o boring_numbers.exe
g++ -Wall -ggdb3 -O0 -std=c++14 boring_numbers.cpp -o boring_numbers.o
boring_numbers.exe < boring_numbers.in

Input:

6
5 15
120 125
779 783
141 19740
61894 7611091
1 10000000

Output:

Case #1: 6
Case #2: 3
Case #3: 2
Case #4: 1365
Case #5: 73250
Case #6: 97655

*/

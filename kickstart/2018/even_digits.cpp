
#include <algorithm>
#include <array>
#include <iostream>
#include <string>

// Even Digits
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050edf/00000000000510ed

static std::array<int64_t, 19> pow10, min_sub;

static void solve() {
    std::string S;
    std::cin >> S;

    int64_t ans{0};
    const auto it = std::find_if(S.begin(), S.end(), [](char c){ return c & 1; });
    if (it != S.end()) {
        S.erase(S.begin(), it);
        if (S.size() == 1)
            ans = 1;
        else {
            const int msd = S[0] - '0';
            const int64_t n = std::atoll(S.c_str());
            const int64_t sz = S.size() - 1;
            const int64_t tail = n % pow10[sz];
            ans = std::min(tail + min_sub[sz - 1], (msd + 1) * pow10[sz + (msd == 9 ? 1 : 0)] - n);
        }
    }

    std::cout << ans;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    std::generate(pow10.begin(), pow10.end(), [f = int64_t{1}]() mutable { 
        const auto n = f; f *= 10; return n; 
    });
    min_sub[0] = 2;
    for (int i = 1; i < 19; ++i)
        min_sub[i] = min_sub[i - 1] + pow10[i];

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++17 even_digits.cpp -o even_digits.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address even_digits.cpp -o even_digits

Run:
even_digits.exe < even_digits.in

Input:

4
42
11
1
2018

Output:

Case #1: 0
Case #2: 3
Case #3: 1
Case #4: 2

*/

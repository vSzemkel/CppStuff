
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

// Palindromic Factors
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008caa74/0000000000acee89

static auto get_all_divisors(const int64_t n) {
    std::vector<int64_t> ans;
    const auto nsq = static_cast<int64_t>(std::sqrt(n));
    for (int i = 1; i <= nsq; ++i) {
        const auto d = n / i;
        if (d * i == n) {
            ans.push_back(i);
            ans.push_back(d);
        }
    }

    const int size = int(ans.size());
    if (size > 1 && ans[size - 2] == ans.back())
        ans.pop_back();

    return ans;
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

static void solve() {
    int64_t N;
    std::cin >> N;

    auto all = get_all_divisors(N);
    std::cout << std::count_if(all.begin(), all.end(), is_num_palindrome<int64_t>);
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 palindromic_factors.cpp -o palindromic_factors.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address palindromic_factors.cpp -o palindromic_factors

Run:
palindromic_factors.exe < palindromic_factors.in

Input:

4
6
10
144
242

Output:

Case #1: 4
Case #2: 3
Case #3: 7
Case #4: 6

*/

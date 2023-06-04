
#include <iostream>

// No Nine
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050ff4/0000000000051183

static bool is_good(const int64_t n) {
    int total{};
    bool has_9{};
    for (auto t = n; t; t /= 10) {
        const auto d = t % 10;
        total += d;
        if (d == 9)
            has_9 = true;
    }

    return total % 9 != 0 && !has_9;
}

/**
 * @Observation1: base9 representation of good numbers is exactly the same as base10
 * @Observation2: we will count be hand good numbers in range [X / 10 * 10 .. X]
 * @Observation3: for every Y there is 8/9*C good numbers in a set {10Y + 0, 10Y + 1, .. 10Y + 8}
 * where C is the number of numbers in range [1 .. Y/10]
 */
static void solve() {
    int64_t A, B;
    std::cin >> A >> B;

    const auto helper = [](const int64_t n) {
        int64_t ans{};
        for (int64_t i = n / 10 * 10; i <= n; ++i)
            if (is_good(i))
                ++ans;

        int64_t c{};
        for (int64_t fac = 9, y = n / 10; y; y /= 10, fac *= 9)
            c += (y % 10) * fac;

        return ans + 8 * c / 9;
    };

    std::cout << helper(B) - helper(A) + 1;
}

static void solve_naive() {
    int64_t A, B;
    std::cin >> A >> B;

    const auto naive_helper = [](const int64_t n) {
        int64_t ans{};
        for (int64_t i = 1; i <= n; ++i)
            if (is_good(i))
                ++ans;

        return ans;
    };

    std::cout << naive_helper(B) - naive_helper(A) + 1;
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
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 no_nine.cpp -o no_nine.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address no_nine.cpp -o no_nine

Run:
no_nine.exe < no_nine.in

Input:

3
16 26
681 834146
188839 91836273993

Output:

Case #1: 9
Case #2: 439606
Case #3: 28560391457

*/

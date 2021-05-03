
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

// Roaring Years
// https://codingcompetitions.withgoogle.com/codejam/round/00000000004362d7/00000000007c0f01

static const int64_t MAX = std::pow(10, 18);
std::vector<int64_t> roars, pow10(19);

const auto dig_cnt = [](const int64_t n){return int(std::log10(n)) + 1;};

static bool is_roaring(const int64_t year) {
    const int digits = dig_cnt(year);
    for (int len = 1; len <= digits / 2; ++len) {
        int64_t start = year / pow10[digits - len];
        int64_t chunk = start, pref = len;
        while (true) {
            if (int64_t(year / pow10[digits - pref]) != start)
                break;
            const int64_t next = ++chunk;
            const int next_digits = dig_cnt(next);
            pref += next_digits;
            if (pref > digits)
                break;
            start = start * pow10[next_digits] + next;
        }

        if (start == year)
            return true;
    }

    return false;
}

static int64_t roaring_in_half(const int64_t year, const int64_t ubound) {
    const int digits_y = dig_cnt(year);
    int64_t chunk = year / pow10[(digits_y + 1) / 2];
    while (true) {
        const int64_t next = chunk + 1;
        const int next_digits = dig_cnt(next);
        int64_t candidate = chunk * pow10[next_digits] + next;
        if (candidate >= ubound || candidate < 0)
            return ubound;
        if (candidate >= year && is_roaring(candidate))
            return candidate;
        ++chunk;
    }
}

static void init() {
    int64_t p{1};
    for (auto& pow : pow10) {
        pow = p;
        p *= 10;
    }

    roars.reserve(2'020'000);
    for (int p = 0; p < 6; ++p) {
        int64_t base = std::pow(10, p);
        for (int64_t start = base; start < 10 * base; ++start) {
            int len{p + 1};
            int64_t chunk{start}, roar{start};
            while (true) {
                const int64_t next = ++chunk;
                const int next_digits = dig_cnt(next);
                len += next_digits;
                if (len >= 19)
                    break;
                roar = roar * pow10[next_digits] + next;
                roars.push_back(roar);
            }
        }
    }

    std::sort(roars.begin(), roars.end());
}

static void solve() {
    int64_t Y; std::cin >> Y;

    int64_t ret = 1234567891011121314;
    if (Y < roars.back()) {
        auto it = std::lower_bound(roars.begin(), roars.end(), Y);
        if (*it == Y) ++it;
        ret = roaring_in_half(Y + 1, *it);
    }

    std::cout << ret;
}

static void solve_nqiii() { // original author: nqiiii
    int64_t Y; std::cin >> Y;
    constexpr auto construct = [](int64_t base, int rep){
        auto ret = base, chunk = base;
        while (rep > 1) {
            const int64_t next = ++chunk;
            const int next_digits = dig_cnt(next);
            if (2 * MAX / pow10[next_digits] < ret)
                return 2 * MAX;
            ret = ret * pow10[next_digits] + next;
            --rep;
        }
        return ret;
    };

    int64_t ans = 2 * MAX;
    for (int rep = 2; rep <= 18; ++rep) {
        int64_t lb{0}, ub{pow10[9]};
        while (lb != ub) {
            const auto mid = (lb + ub) / 2;
            if (construct(mid, rep) > Y)
                ub = mid;
            else
                lb = mid + 1;
        }
        ans = std::min(ans, construct(lb, rep));
    }

    std::cout << ans;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    init();

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 roaring_years.cpp -o roaring_years.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address roaring_years.cpp -o roaring_years.o

Run:
py.exe interactive_runner.py py.exe roaring_years_testing_tool.py 1 -- roaring_years.exe
roaring_years.exe < roaring_years.in

Input:

4
2020
2021
68000
101

Output:


*/

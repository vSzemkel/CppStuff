
#include <array>
#include <cassert>
#include <iostream>
#include <numeric>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

// Find maximal number of points on a line
// https://leetcode.com/problems/max-points-on-a-line


// Fraction implemented on integral type T

template <class T = int64_t>
struct fraction_t {
    inline bool is_zero() const { return num == 0 || denum == 0; }

    void reduce() {
        if (!is_zero()) {
            const auto gcd = std::gcd(num, denum);
            num /= gcd;
            denum /= gcd;
            if (num < 0 && denum < 0) {
                num = -num;
                denum = -denum;
            }
        } else {
            num = 0;
            denum = 1;
        }
    }

    operator double() const { return double(num) / denum; }

    void operator+=(const fraction_t& other) {
        if (!other.is_zero()) {
            if (is_zero()) {
                num = other.num;
                denum = other.denum;
            } else {
                num = num * other.denum + other.num * denum;
                denum *= other.denum;
            }
            reduce();
        }
    }

    void operator-=(const fraction_t& other) {
        if (!other.is_zero()) {
            if (is_zero()) {
                num = -other.num;
                denum = other.denum;
            } else {
                num = num * other.denum -    other.num * denum;
                denum *= other.denum;
            }
            reduce();
        }
    }

    void operator*=(const fraction_t& other) {
        if (!other.is_zero()) {
            if (!is_zero()) {
                num *= other.num;
                denum *= other.denum;
                reduce();
            }
        } else {
            num = 0;
            denum = 1;
        }
    }

    void operator/=(const fraction_t& other) {
        if (!other.is_zero()) 
            operator*=({other.denum, other.num});
    }

    std::array<std::string, 2> to_decimal_string() { // ret[1] is decimal period digits
        assert(denum > 0);
        auto pref = std::to_string(num / denum);
        const int rem = num % denum;
        if (rem == 0)
            return { pref + ".0", {} };

        pref.push_back('.');
        int counter{0};
        int val = rem * 10;
        std::unordered_map<int, int> state;
        while (true) {
            const auto found = state.find(val);
            if (found != state.end()) {
                const int pref_len = pref.size() - counter + found->second;
                const auto period = pref.substr(pref_len);
                pref.resize(pref_len);
                return { pref, period };
            }

            const int next_digit = val / denum;
            pref.push_back('0' + next_digit);
            if (val % denum == 0)
                return { pref, {} };
            state[val] = counter++;
            val = (val - next_digit * denum) * 10;
        }
    }

    friend fraction_t operator+(const fraction_t& f1, const fraction_t& f2) { auto ret = f1; ret += f2; return ret; }
    friend fraction_t operator-(const fraction_t& f1, const fraction_t& f2) { auto ret = f1; ret -= f2; return ret; }
    friend fraction_t operator*(const fraction_t& f1, const fraction_t& f2) { auto ret = f1; ret *= f2; return ret; }
    friend fraction_t operator/(const fraction_t& f1, const fraction_t& f2) { auto ret = f1; ret /= f2; return ret; }

    friend bool operator==(const fraction_t& f1, const fraction_t& f2) {
        auto tf1 = f1;
        auto tf2 = f2;
        tf1.reduce();
        tf2.reduce();
        return tf1.num == tf2.num && tf1.denum == tf2.denum;
    }

    friend bool operator<(const fraction_t& f1, const fraction_t& f2) {
        return (__int128_t)f1.num * f2.denum < (__int128_t)f2.num * f1.denum;
    }

    friend std::ostream& operator<<(std::ostream& os, const fraction_t& f) {
        return os << f.num << '/' << f.denum;
    }

    friend struct std::hash<fraction_t<T>>;

    T num, denum{1};
};

template <class T>
struct std::hash<fraction_t<T>> {
    std::size_t operator()(const fraction_t<T>& f) const noexcept {
        auto rf = f;
        rf.reduce();
        const auto h1 = std::hash<T>{}(rf.num);
        const auto h2 = std::hash<T>{}(rf.denum);
        return h1 ^ (h2 << 1);
    }
};

using frac_t = fraction_t<>;

/**
 * @brief Every two points forms a section. Every section is on some line
 * Every line either is crossing y axis and not x axis or is crossing x axis
 * It is sufficient to find maximal number of crossing a single axis
 * 
 * For every N colinear points there are N * (N - 1) crossings of some axis (or both)
 */
static int maxPoints(std::vector<std::vector<int>>& points)
{
    std::unordered_map<int, int> y_crossings;
    std::unordered_map<frac_t, std::unordered_map<frac_t, int>> normal_crossings;

    const auto N = int(points.size());
    if (N <= 2)
        return N;

    std::sort(points.begin(), points.end());
    for (int i = 0; i < N - 1; ++i)
        for (int j = i + 1; j < N; ++j)
            if (points[i][1] == points[j][1])
                ++y_crossings[points[j][1]];
            else {
                // x = x1 - ((y1 * (x2 - x1)) / (y2 - y1))
                const auto dx = points[j][0] - points[i][0];
                const auto dy = points[j][1] - points[i][1];
                const frac_t x = frac_t{points[i][0], 1} - frac_t{points[i][1] * dx, dy};
                ++normal_crossings[frac_t{dy, dx}][x];
            }

    int ans{};
    for (const auto& [_, c] : y_crossings)
        ans = std::max(ans, c);
    for (const auto& [_, l1] : normal_crossings)
        for (const auto& [_, c] : l1)
        ans = std::max(ans, c);

    int count{2};
    while (count * (count - 1) / 2 < ans)
        ++count;

    return count;
}

static void io_handler()
{
    int N;
    std::cin >> N;
    std::vector<std::vector<int>> points(N, std::vector<int>(2));
    for (auto& p : points)
        std::cin >> p[0] >> p[1];

    std::cout << maxPoints(points);
}

int main(int, char**)
{
    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": ";
        io_handler();
        std::cout << '\n';
    }
}

/*

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 max_points_on_a_line.cpp -o max_points_on_a_line.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address max_points_on_a_line.cpp -o n_queens-ii

Input

1
6
4 5
5 6
6 7
7 8
3 7
6 7

Output

Case #1: 4

*/

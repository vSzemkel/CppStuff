/*
ID: marcin.8
LANG: C++
TASK: ratios
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=R4uJzFQ8B9a&S=ratios
*/

#include <algorithm>
#include <array>
#include <assert.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

std::ifstream task_in("ratios.in");
std::ofstream task_out("ratios.out");

constexpr const int INF = 1e9;
constexpr const double EPS = 1e-9;

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
        return (int64_t)f1.num * f2.denum < (int64_t)f2.num * f1.denum;
    }

    friend std::ostream& operator<<(std::ostream& os, const fraction_t& f) {
        return os << f.num << '/' << f.denum;
    }

    T num, denum{1};
};

template <int R, int C, typename T = int>
struct matrix_t {
    matrix_t() {
        for (int r = 0; r < R; ++r)
            _m[r].fill(T{});
    }

    matrix_t(const std::initializer_list<T>& data) {
        assert(data.size() == R * C);
        for (int r = 0; r < R; ++r)
            std::copy_n(data.begin() + r * C, C, _m[r].begin());
    }

    int rows() const { return R; }
    int cols() const { return C; }

    template <int X>
    auto operator*(const matrix_t<C, X, T>& rhs) const {
        matrix_t<R, X, T> ret;
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < rhs.cols(); ++c) {
                T x{};
                for (int i = 0; i < C; ++i)
                    x += _m[r][i] * rhs[i][c];
                ret[r][c] = x;
            }
        return ret;
    }

    const auto& operator[] (const int row) const {
        assert(0 <= row && row < R);
        return _m[row];
    }

    auto& operator[](const int row) {
        assert(0 <= row && row < R);
        return _m[row];
    }

    void print() const {
        for (int r = 0; r < R; ++r) {
            for (int c = 0; c < C; ++c)
                std::cout << _m[r][c] << ' ';
            std::cout << '\n';
        }
        std::cout << std::endl;
    }

    /**
     * @brief In the last column of this matix we have RHS of linear equation
     * @see https://cp-algorithms.com/linear_algebra/linear-system-gauss.html
     * @param ans Placeholder for the solution
     * @return int Solution existence: -1:NO, 0:single, 1:INF
     */
    int gauss(std::vector<T>& ans) {
        const int anssz = C - 1;
        std::vector<int> where(anssz, -1);
        for (int c = 0, r = 0; c < anssz && r < R; ++c) {
            int sel{r};
            for (int i = r; i < R; ++i)
                if (std::abs(_m[i][c]) > std::abs(_m[sel][c]))
                    sel = i;
            if (std::abs(_m[sel][c]) < EPS)
                continue;
            std::swap(_m[sel], _m[r]);
            where[c] = r;

            for (int i = 0; i < R; ++i)
                if (i != r) {
                    const auto f = _m[i][c] / _m[r][c];
                    for (int j = c; j < C; ++j)
                        _m[i][j] -= _m[r][j] * f;
                }
            ++r;
        }

        ans.resize(anssz);
        for (int c = 0; c < anssz; ++c) {
            const int w = where[c];
            if (w != -1)
                ans[c] = _m[w][anssz] / _m[w][c];
        }
        for (int r = 0; r < R; ++r) {
            T sum{};
            for (int c = 0; c < anssz; ++c)
                sum += ans[c] * _m[r][c];
            if (std::abs(sum - _m[r][anssz]) > EPS)
                return 0;
        }
        for (int i = 0; i < anssz; ++i)
            if (where[i] == -1)
                return INF;
        return 1;
    }

  private:
    std::array<std::array<T, C>, R> _m;
};

int main(int, char**)
{
    int a, b, c;
    matrix_t<3, 4, fraction_t<int>> problem;
    task_in >> a >> b >> c;
    problem[0][3] = fraction_t<int>{a}; problem[1][3] = fraction_t<int>{b}; problem[2][3] = fraction_t<int>{c};
    for (int n = 0; n < 3; ++n) {
        task_in >> a >> b >> c;
        problem[0][n] = fraction_t<int>{a}; problem[1][n] = fraction_t<int>{b}; problem[2][n] = fraction_t<int>{c};
    }

    std::vector<fraction_t<int>> ans;
    if (problem.gauss(ans) != 1 || std::any_of(ans.begin(), ans.end(), [](const auto a){ return a < 0; }))
        task_out << "NONE\n";
    else {
        int factor = ans[0].denum;
        for (int i = 0; i < 3; ++i)
            factor = std::lcm(factor, ans[i].denum);
        for (int i = 0; i < 3; ++i)
            task_out << (factor * ans[i].num / ans[i].denum) << ' ';
        task_out << factor << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 ratios.cpp -o ratios.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address ratios.cpp -o ratios

Input:

3 4 5
1 2 3
3 7 1
2 1 2

Output:

8 1 5 7

*/

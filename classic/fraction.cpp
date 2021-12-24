
#include <array>
#include <cassert>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

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

    T num, denum;
};

using frac_t = fraction_t<>;

int main(int, char**)
{

    frac_t f{1, 2};
    f += {1, 4};
    f += {2, 8};
    assert((f == frac_t{1, 1}));

    f -= {3, 7};
    assert((f == frac_t{4, 7}));

    frac_t f2{2, 3};
    f2 *= {4, 7};
    assert((f2 == frac_t{8, 21}));
    assert((f2 < frac_t{9, 21}));

    frac_t f3{8, 14};
    f3 /= {4, 7};
    assert((f3 == frac_t{1, 1}));

    frac_t f4{31, 13};
    const auto decim = f4.to_decimal_string();
    assert(decim[0] == "2." && decim[1] == "384615");
}

/* Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 fraction.cpp -o fraction.exe
*/

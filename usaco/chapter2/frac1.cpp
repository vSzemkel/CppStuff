/*
ID: marcin.8
LANG: C++
TASK: frac1
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=WoNEtXFVIfb&S=frac1
*/

#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>
#include <numeric>
#include <string>
#include <vector>

std::ifstream task_in("frac1.in");
std::ofstream task_out("frac1.out");

template <typename T>
T GCD(T u, T v) { // USACO compiler has not access to std::gcd
    while (v != 0) {
        auto r = u % v;
        u = v;
        v = r;
    }
    return u;
}

template <class T = int64_t>
struct fraction_t {
    inline bool is_zero() const { return num == 0 || denum == 0; }

    void reduce() {
        if (!is_zero()) {
            const auto gcd = GCD(num, denum);
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
        return /*(__int128_t)*/f1.num * f2.denum < /*(__int128_t)*/f2.num * f1.denum;
    }

    friend std::ostream& operator<<(std::ostream& os, const fraction_t& f) {
        return os << f.num << '/' << f.denum;
    }

    T num, denum;
};

void greedy(int, char**)
{
    int N; task_in >> N;
    std::vector<fraction_t<int>> frac;
    for (int denum = 1; denum <= N; ++denum)
        for (int num = 0; num < denum; ++num) {
            fraction_t<int> f{num, denum};
            f.reduce();
            frac.push_back(f);
        }

    std::sort(frac.begin(), frac.end());
    const auto ne = std::unique(frac.begin(), frac.end());
    frac.erase(ne, frac.end());

    for (const auto& f : frac)
        task_out << f << '\n';
    task_out << "1/1\n";
}

int N;

void div_conq(const fraction_t<int>& b, const fraction_t<int>& e) {
    if (b.denum + e.denum > N)
        return;
    const fraction_t<int> mid{b.num + e.num, b.denum + e.denum};
    div_conq(b, mid);
    task_out << mid << '\n';
    div_conq(mid, e);
}

int main(int, char**)
{
    task_in >> N;
    task_out << "0/1\n";
    div_conq(fraction_t<int>{0, 1}, fraction_t<int>{1, 1});
    task_out << "1/1\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 frac1.cpp -o frac1.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address frac1.cpp -o frac1

Input:

5

Output:

0/1
1/5
1/4
1/3
2/5
1/2
3/5
2/3
3/4
4/5
1/1

*/

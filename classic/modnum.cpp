
#include <assert.h>
#include <iostream>
#include <optional>
#include <vector>

// Modnum - integral datatype implementing modulo arithmetic

template <typename T = int, T M = 998244353>
class modnum_t {
  public:
    modnum_t() : value(0) {}
    modnum_t(T v) : value(v % M) {
        if (value < 0) value += M;
    }

    modnum_t inv() const // not exists if < 0
    {
        modnum_t res;
        res.value = minv(value, M);
        return res;
    }

    modnum_t neg() const
    {
        return modnum_t{value ? M - value : 0};
    }

    modnum_t operator-() const
    {
        return neg();
    }

    modnum_t operator+() const
    {
        return modnum_t(*this);
    }

    modnum_t& operator+=(const modnum_t& o)
    {
        value += o.value;
        if (value >= M) value -= M;
        return *this;
    }

    modnum_t& operator-=(const modnum_t& o)
    {
        value -= o.value;
        if (value < 0) value += M;
        return *this;
    }

    modnum_t& operator*=(const modnum_t& o)
    {
        value = modmul(value, o.value);
        return *this;
    }

    modnum_t& operator/=(const modnum_t& o)
    {
        return *this *= o.inv();
    }

    modnum_t& operator++()
    {
        value++;
        if (value == M) value = 0;
        return *this;
    }

    modnum_t& operator--()
    {
        if (value == 0) value = M;
        value--;
        return *this;
    }

    friend modnum_t operator++(modnum_t& a, int)
    {
        modnum_t r = a;
        ++a;
        return r;
    }

    friend modnum_t operator--(modnum_t& a, int)
    {
        modnum_t r = a;
        --a;
        return r;
    }

    friend std::optional<modnum_t> inv(const modnum_t& m) { const auto ret = m.inv(); return ret.value < 0 ? std::nullopt : std::optional{ret}; }
    friend modnum_t neg(const modnum_t& m) { return m.neg(); }

    friend std::ostream& operator<<(std::ostream& out, const modnum_t& n) { return out << T(n); }
    friend std::istream& operator>>(std::istream& in, modnum_t& n) { T v; in >> v; n = modnum_t(v); return in; }

    friend bool operator<(const modnum_t& a, const modnum_t& b) { return a.value < b.value; }
    friend bool operator==(const modnum_t& a, const modnum_t& b) { return a.value == b.value; }
    friend bool operator!=(const modnum_t& a, const modnum_t& b) { return a.value != b.value; }
    friend modnum_t operator+(const modnum_t& a, const modnum_t& b) { return modnum_t(a) += b; }
    friend modnum_t operator-(const modnum_t& a, const modnum_t& b) { return modnum_t(a) -= b; }
    friend modnum_t operator*(const modnum_t& a, const modnum_t& b) { return modnum_t(a) *= b; }
    friend modnum_t operator/(const modnum_t& a, const modnum_t& b) { return modnum_t(a) /= b; }
    friend T operator%(const modnum_t& a, const T b) { return a.value % b; }

    explicit operator int() const { return int(value); }
    explicit operator int64_t() const { return int64_t(value); }

    static modnum_t bin_coeff(const int n, const int k)
    {
        modnum_t ret{1};
        for (int i = 0; i < k; ++i)
            ret = (ret * (n - i)) / (i + 1);
        return ret;
    }

    static T add(T a, T b)
    {
        a += b;
        while (a >= M)
            a -= M;
        return a;
    }

    static T modmul(T a, T b)
    {
        int64_t r{0};
        while (b) {
            if (b & 1) r = add(r, a);
            b >>= 1;
            a = add(a, a);
        }
        return r;
    }

    static T range_len(const int a, const int b) { // [a..b]
        auto len = b - a + 1;
        if (len <= 0)
            len = (M + 1 - a + b) % M;
        return len;
    }

  private:
    static T minv(T a, const T m) {
        a %= m;
        if (a == 0) return -1; // not exists
        return a == 1 ? 1 : T(m - int64_t(minv(m, a)) * int64_t(m) / a);
    }

    T value;
};

int main(int, char**)
{
    modnum_t<int, 20> m = 13;
    assert(inv(m).value() == 17); // 13 * 17 == 1 (mod 20)
    modnum_t<int64_t, 20> m2 = 15;
    assert(!inv(m2).has_value());

    assert(neg(m2) == 5);
    auto m3 = m2++;
    assert(m2 == 16 && m3 == 15);
    assert(m2 * m3 == 0);

    modnum_t<int64_t> m4;
    assert(m4.modmul(16351848, 9174561) == 872592476);
    assert(m4.bin_coeff(197, 8) == 26874261); // 48717346033720 % 998244353

    modnum_t<int, 360> wheel;
    assert(wheel.range_len(100, 200) == 101);
    assert(wheel.range_len(200, 100) == 261); // 200..359 0..100 -> 160 + 101

    std::vector<int> fact, inv_fact, inverses;
    const auto init_fact = [&](const size_t max, const int M) {
        fact.resize(max);
        inv_fact.resize(max);
        inverses.resize(max);
        inverses[1] = fact[1] = inv_fact[1] = fact[0] = inv_fact[0] = 1;
        for (size_t i = 2; i < max; ++i) {
            inverses[i] = (int)((int64_t)(M - M / i) * inverses[M % i] % M);
            inv_fact[i] = (int)((int64_t)inv_fact[i - 1] * inverses[i] % M);
            fact[i] = (int)((int64_t)fact[i - 1] * i % M);
        }
    };

    modnum_t<int> invfac = 15;
    init_fact(100, 998244353);
    assert(fact[20] == 401576539);
    assert(inverses[15] == invfac.inv());
    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 modnum.cpp -o modnum.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 modnum.cpp -o modnum.o

*/

#include <assert.h>
#include <iostream>
#include <optional>

// Modnum - 64 bit integral datatype implementing modulo arithmetic

template <int64_t M = 998244353>
class modnum_t {
  public:
    modnum_t() : value(0) {}
    modnum_t(int64_t v) : value(v % M) {
        if (value < 0) value += M;
    }

    modnum_t inv() const
    {
        modnum_t res;
        res.value = minv(value, M);
        return res;
    }

    modnum_t neg() const
    {
        modnum_t res;
        res.value = value ? M - value : 0;
        return res;
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

    friend std::ostream& operator<<(std::ostream& out, const modnum_t& n) { return out << int64_t(n); }
    friend std::istream& operator>>(std::istream& in, modnum_t& n) { int64_t v; in >> v; n = modnum_t(v); return in; }

    friend bool operator<(const modnum_t& a, const modnum_t& b) { return a.value < b.value; }
    friend bool operator==(const modnum_t& a, const modnum_t& b) { return a.value == b.value; }
    friend bool operator!=(const modnum_t& a, const modnum_t& b) { return a.value != b.value; }
    friend modnum_t operator+(const modnum_t& a, const modnum_t& b) { return modnum_t(a) += b; }
    friend modnum_t operator-(const modnum_t& a, const modnum_t& b) { return modnum_t(a) -= b; }
    friend modnum_t operator*(const modnum_t& a, const modnum_t& b) { return modnum_t(a) *= b; }
    friend modnum_t operator/(const modnum_t& a, const modnum_t& b) { return modnum_t(a) /= b; }
    friend int64_t operator%(const modnum_t& a, const int64_t b) { return a.value % b; }

    explicit operator int64_t() const { return value; }

  private:
    static int64_t add(int64_t a, int64_t b)
    {
        a += b;
        while (a >= M)
            a -= M;
        return a;
    }

    static int64_t modmul(int64_t a, int64_t b)
    {
        int64_t r{0};
        while (b) {
            if (b & 1) r = add(r, a);
            b >>= 1;
            a = add(a, a);
        }
        return r;
    }

    static int64_t minv(int64_t a, const int64_t m) {
        a %= m;
        if (a == 0) return -1; // not exists
        return a == 1 ? 1 : int64_t(m - int64_t(minv(m, a)) * int64_t(m) / a);
    }

    int64_t value;
};


int main(int, char**)
{
    modnum_t<20> m = 13;
    assert((int64_t)inv(m).value() == 17); // 13 * 17 == 1 (mod 20)
    modnum_t<20> m2 = 15;
    assert(!inv(m2).has_value());

    assert((int64_t)neg(m2) == 5);
    auto m3 = m2++;
    assert((int64_t)m2 == 16 && (int64_t)m3 == 15);
    assert((int64_t)(m2 * m3) == 0);
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 modnum64.cpp -o modnum64.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 modnum64.cpp -o modnum64.o

*/
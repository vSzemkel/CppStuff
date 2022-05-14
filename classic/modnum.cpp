
#include <array>
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

    modnum_t operator^(const int n) const
    {
        return modnum_t{pow(value, n)};
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
        value = mul(value, o.value);
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

    static T mul(T a, T b)
    {
        int64_t r{0};
        while (b) {
            if (b & 1) r = add(r, a);
            b >>= 1;
            a = add(a, a);
        }
        return r;
    }

    static T pow(T a, T n)
    {
        modnum_t b{a}, ret{1};
        while (n > 0) {
            if (n & 1)
                ret *= b;
            b = b * b;
            n >>= 1;
        }
        return T(ret);
    }

    static T catalan(T n)
    {
        return T(bin_coeff(2 * n, n) * pow(n + 1, M - 2));
    }

    static T range_len(const int a, const int b) { // [a..b]
        auto len = b - a + 1;
        if (len <= 0)
            len = (M + 1 - a + b) % M;
        return len;
    }

    // a1 = ax + by == std::gcd(a, b)
    // https://cp-algorithms.com/algebra/extended-euclid-algorithm.html#iterative-version
    static auto euclid(const T a, const T b) {
        T x{1}, y{0}, x1{0}, y1{1}, a1{a}, b1{b};
        while (b1) {
            const T q = a1 / b1;
            std::tie(x, x1)  = std::make_tuple(x1, x - q * x1);
            std::tie(y, y1)  = std::make_tuple(y1, y - q * y1);
            std::tie(a1, b1) = std::make_tuple(b1, a1 - q * b1);
        }
        return std::make_tuple(a1, x, y);
    }

    // find x: (ax = b) mod M
    // returns {exists, {solutions}}
    static std::pair<bool, std::vector<T>> solve(const T a, const T b) {
        const auto [g, x, _] = euclid(a, M);
        if (b % g)
            return {false, {}};
        std::vector<T> ans(g);
        const T x0{x * b / g}, s{M / g};
        T p{x0 - s};
        for (auto& a : ans) {
            a = p + s;
            p = a;
        }
        return {true, ans};
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
    assert(m4.mul(16351848, 9174561) == 872592476);
    assert(m4.bin_coeff(197, 8) == 26874261); // 48717346033720 % 998244353

    modnum_t<int, 360> wheel;
    assert(wheel.range_len(100, 200) == 101);
    assert(wheel.range_len(200, 100) == 261); // 200..359 0..100 -> 160 + 101

    std::vector<modnum_t<>> fact, inv_fact, inverses;
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

    modnum_t<> modpow = 158715;
    assert((modpow ^ 4) == (modpow * modpow * modpow * modpow));

    const auto c = modnum_t<>::catalan(15);
    assert(c == 9694845);

    const auto euclid = modnum_t<>::euclid(35, 50);
    assert((euclid == std::make_tuple(5, 3, -2)));

    modnum_t<int, 50> solver;
    const auto s = solver.solve(35, 10);
    assert((s.second == std::vector<int>{6 ,16, 26, 36, 46}));

    modnum_t<> invfac = 15;
    init_fact(100, 998244353);
    assert(fact[20] == 401576539);
    assert(inverses[15] == invfac.inv());
    assert(fact[17] * inv_fact[10] * inv_fact[17 - 10] == invfac.bin_coeff(17, 10));
    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 modnum.cpp -o modnum.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 modnum.cpp -o modnum

*/
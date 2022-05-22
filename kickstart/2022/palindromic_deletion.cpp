
#include <algorithm>
#include <array>
#include <assert.h>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

// Palindromic Deletion
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb4d1/0000000000b20d16


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

bool is_palindrome2(const std::string& s, const std::string& m)
{
    int l = 0, r = s.size() - 1;
    while (l < r) { // m has '1' for sure
        while (m[l] == '0') ++l;
        while (m[r] == '0') --r;
        if (s[l++] != s[r--]) return false;
    }
    return true;
}

static void solve_set1() { // TLE
    int N;
    std::string S;
    std::cin >> N >> S;

    auto perm = std::string(N, '1');
    modnum_t<int, 1000000007> ans{2};
    for (int len = N - 1; len > 1; --len) {
        perm[N - len - 1] = '0';
        const auto weight = ans.bin_coeff(N, len).inv();
        int cnt{0};
        do {
            if (is_palindrome2(S, perm))
                ++cnt;
        } while (std::next_permutation(perm.begin(), perm.end()));
        ans += cnt * weight;
    }

    std::cout << ans;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve_set1(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 palindromic_deletion.cpp -o palindromic_deletion.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address palindromic_deletion.cpp -o palindromic_deletion

Run:
palindromic_deletion.exe < palindromic_deletion.in

Input:

2
2
ab
3
aba

Output:

Case #1: 2
Case #2: 333333338

*/

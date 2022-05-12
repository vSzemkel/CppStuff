
#include <iostream>
#include <optional>
#include <vector>

// Intranets
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000877b42/0000000000afeb38#analysis

template <typename T = int, T M = 1'000'000'007>
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

/* This is digest from the solution for Set2

def catalan(n):
    return binom(2 * n, n) * pow(n + 1, MOD - 2, MOD) % MOD

def numer(n, k):
    ans = pow(2, (n - 2 * k - 1), MOD)
    ans *= binom(n - 1, 2 * k)
    ans %= MOD
    ans *= binom(2 * k, k)
    ans %= MOD
    ans *= pow(k + 1, MOD - 2, MOD)
    ans %= MOD
    return ans

def solve():
    M, K = rrm()
    ans = numer(M - 1, K - 1)
    ans *= pow(catalan(M - 1), MOD - 2, MOD)
    ans %= MOD
    return ans
*/

static void solve_set1() {
    int M, K;
    std::cin >> M >> K;

    // we label with decreasing priorities
    // dp[m][k] probability that after labeling some initial edges 
    // there are m machines connected and they form exactly k intranets
    // dp[m][k] is in fact the sum for possibly many initial labelling
    // consider last added edge
    // CASE1: it connects two not connected machines with probability P1
    // dp[m][k] += dp[m - 2][k - 1] * P1
    // CASE2: it connects one connected machine with disconnected one with probability P2
    // dp[m][k] += dp[m - 1][k] * P2
    std::vector<std::vector<modnum_t<>>> dp(M + 2, std::vector<modnum_t<>>(K + 2, 0));
    dp[0][0] = 1;
    for (int i = 0; i < M; ++i) { // i is number of connected machines
        const modnum_t<> c1 = (M - i) * (M - i - 1) / 2;
        const modnum_t<> c2 = i * (M - i);
        const auto all = c1 + c2;
        const auto p1 = c1 / all;
        const auto p2 = c2 / all;
        for (int j = 0; j <= K; ++j) {
            dp[i + 2][j + 1] += dp[i][j] * p1;
            dp[i + 1][j] += dp[i][j] * p2;
        }
    }

    std::cout << dp[M][K];
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
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 intranets.cpp -o intranets.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address intranets.cpp -o intranets

Run:
intranets.exe < intranets.in

Input:

3
5 2
5 1
6 3

Output:


*/

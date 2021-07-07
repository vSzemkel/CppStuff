
#include <array>
#include <assert.h>
#include <iostream>
#include <optional>
#include <vector>

// Hidden pancakes
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000435915/00000000007dc20c

template <int M = 1'000'000'007>
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
        value = int(int64_t(value) * int64_t(o.value) % M);
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

    friend std::ostream& operator<<(std::ostream& out, const modnum_t& n) { return out << int(n); }
    friend std::istream& operator>>(std::istream& in, modnum_t& n) { int64_t v; in >> v; n = modnum_t(v); return in; }

    friend bool operator<(const modnum_t& a, const modnum_t& b) { return a.value < b.value; }
    friend bool operator==(const modnum_t& a, const modnum_t& b) { return a.value == b.value; }
    friend bool operator!=(const modnum_t& a, const modnum_t& b) { return a.value != b.value; }
    friend modnum_t operator+(const modnum_t& a, const modnum_t& b) { return modnum_t(a) += b; }
    friend modnum_t operator-(const modnum_t& a, const modnum_t& b) { return modnum_t(a) -= b; }
    friend modnum_t operator*(const modnum_t& a, const modnum_t& b) { return modnum_t(a) *= b; }
    friend modnum_t operator/(const modnum_t& a, const modnum_t& b) { return modnum_t(a) /= b; }
    friend int operator%(const modnum_t& a, const int b) { return a.value % b; }

    explicit operator int() const { return value; }

  private:
    static int minv(int a, const int m) {
        a %= m;
        if (a == 0) return -1; // not exists
        return a == 1 ? 1 : int(m - int64_t(minv(m, a)) * int64_t(m) / a);
    }

    int value;
};

constexpr int MAXN = 100'005;

std::vector<int> g_visible;
std::array<modnum_t<>, MAXN> g_factiorial;

static modnum_t<> compute(const int begin, const int end, const int level) {
    modnum_t ret{1};
    if (begin + 1 < end) {
        const int size{end - begin};
        int pos{end - 1};
        while (begin <= pos && g_visible[pos] != level)
            --pos;
        assert(begin <= pos);
        const int chunk = end - pos;
        ret *= g_factiorial[size - 1] * (g_factiorial[size - chunk] * g_factiorial[chunk - 1]).inv();
        ret *= compute(begin, pos, level);
        ret *= compute(pos + 1, end, level + 1);
    }

    return ret;
}

static int solve() {
    int N; std::cin >> N;
    g_visible.resize(N);
    for (auto& p : g_visible)
        std::cin >> p;

    for (int i = 1; i < N; ++i)
        if (g_visible[i - 1] + 1 < g_visible[i])
            return 0;

    return int(compute(0, N, 1));
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    // precompute factorials
    g_factiorial[0] = 1;
    for (int i = 1; i < MAXN; ++i)
        g_factiorial[i] = g_factiorial[i - 1] * i;

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g)
        std::cout << "Case #" << g << ": " << solve() << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 hidden_pancakes.cpp -o hidden_pancakes.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address hidden_pancakes.cpp -o hidden_pancakes

Run:
hidden_pancakes.exe < hidden_pancakes.in

Input:

4
4
1 2 2 1
3
1 1 2
3
1 1 3
24
1 2 1 2 1 2 1 2 1 2 1 2 1 2 1 2 1 2 1 2 1 2 1 2

Output:

Case #1: 1
Case #2: 2
Case #3: 0
Case #4: 234141013

*/


#include <assert.h>
#include <iostream>
#include <numeric>
#include <optional>
#include <string>
#include <utility>
#include <vector>

// Polynomial rolling hasher
// Using modnum struct of ecnerwala

template <int M>
struct modnum_t {
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

    explicit operator int() const { return value; }

  private:
    static int minv(int a, const int m) {
        a %= m;
        if (a == 0) return -1; // not exists
        return a == 1 ? 1 : int(m - int64_t(minv(m, a)) * int64_t(m) / a);
    }

    int value;
};

template <int BASE = 31> // 53 for mixed case
class string_hasher
{
    using modn_t = modnum_t<998244353>;
  public:
    string_hasher(const std::string& s) : _size((int)s.size()) {
        assert(!s.empty());
        init_powers(_size);
        _prefix_hash.resize(_size);
        for (int i = 0; i < _size; ++i)
            _prefix_hash[i] = POWERS[i] * (s[i] - 'A' + 1);
        std::inclusive_scan(_prefix_hash.begin(), _prefix_hash.end(), _prefix_hash.begin());
    }

    int hash(const int begin, const int end) const { // [begin, end)
        assert(0 <= begin && begin < end && end <= _size);
        if (begin == 0) return (int)_prefix_hash[end - 1];
        return (int)((_prefix_hash[end - 1] - _prefix_hash[begin - 1]) * POWERS[begin].inv());
    }

    int hash() const {
        return (int)_prefix_hash.back();
    }

    friend bool operator==(const string_hasher& a, const string_hasher& b) {
        return a._size == b._size && a._prefix_hash.back() == b._prefix_hash.back();
    }

    friend bool operator<(const string_hasher& a, const string_hasher& b) {
        return std::make_pair(a._size, a._prefix_hash.back()) < std::make_pair(b._size, b._prefix_hash.back());
    }

  private:
    void init_powers(const int len) {
        int psize = (int)POWERS.size();
        if (psize < len + 1) {
            POWERS.reserve(len + 1);
            if (POWERS.empty()) {
                POWERS.push_back(1);
                ++psize;
            }
            while (psize < len + 1) {
                POWERS.push_back(POWERS.back() * BASE);
                ++psize;
            }
        }
    }

    int _size;
    std::vector<modn_t> _prefix_hash;
    static inline std::vector<modn_t> POWERS;
};

int main(int, char**)
{
    string_hasher sh1{"ALA MA KOTA BUREGO"};
    string_hasher sh2{"KOTA"};
    assert(sh1.hash(7, 11) == sh2.hash());
    string_hasher sh3{"clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 polynomial_hasher.cpp -o polynomial_hasher.exe"};
    string_hasher sh4{"polynomial"};
    assert(sh3.hash(48, 58) == sh4.hash());
    string_hasher sh5{"ecnerwala"};
    string_hasher sh6{"ecnerwala"};
    assert(sh5 == sh6);
    string_hasher sh7{"ABC"};
    string_hasher sh8{"ACC"};
    assert(sh7 < sh8);
    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 polynomial_hasher.cpp -o polynomial_hasher.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 polynomial_hasher.cpp -o polynomial_hasher.o

Run:
polynomial_hasher.exe

*/

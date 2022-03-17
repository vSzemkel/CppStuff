/*
ID: marcin.8
LANG: C++
TASK: lgame
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=NbC8CVUxWYb&S=lgame
*/

#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iterator>
#include <numeric>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

std::ifstream task_in("lgame.in");
std::ofstream task_out("lgame.out");

std::unordered_map<char, int> charval = {
    {'a', 2}, {'b', 5}, {'c', 4}, {'d', 4}, {'e', 1}, {'f', 6},
    {'g', 5}, {'h', 5}, {'i', 1}, {'j', 7}, {'k', 6}, {'l', 3},
    {'m', 5}, {'n', 2}, {'o', 3}, {'p', 5}, {'q', 7}, {'r', 2},
    {'s', 1}, {'t', 2}, {'u', 4}, {'v', 6}, {'w', 6}, {'x', 7},
    {'y', 5}, {'z', 7}
};

template <int M>
struct modnum_t {
  public:
    modnum_t() : value(0) {}
    modnum_t(int v) : value(v % M) {
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

    friend modnum_t inv(const modnum_t& m) { const auto ret = m.inv(); return ret.value < 0 ? 0 : ret; }
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
        std::partial_sum(_prefix_hash.begin(), _prefix_hash.end(), _prefix_hash.begin());
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

    static modn_t roll(modn_t hash, const char old_char, const char new_char, const int len) {
        hash -= (old_char - 'A' + 1);
        hash /= BASE;
        hash += POWERS[len - 1] * (new_char - 'A' + 1);
        return hash;
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
    int total{0};
    std::string input;
    task_in >> input;
    for (const char c : input)
        total += charval[c];

    std::string word;
    std::vector<std::string> dict;
    std::ifstream src{"lgame.dict"};
    while (std::getline(src, word))
        dict.push_back(word);

    std::string weighted_input = input;
    std::sort(weighted_input.begin(), weighted_input.end(), [](const char c1, const char c2){
        return charval[c1] > charval[c2];
    });

    int best{0}, size = (1 << int(input.size()));
    std::vector<std::pair<std::string, int>> found; // {string, space position}
    for (int mask = 0; mask < size; ++mask) {
        // remove masked out letters starting from least valuable
        int bit{1}, ans{total};
        std::string letters;
        std::vector<std::pair<std::string, int>> round_findings;
        for (const char c : weighted_input) {
            if ((mask & bit) == 0)
                letters.push_back(c);
            else
                ans -= charval[c];
            bit <<= 1;
        }
        if (ans < best || letters.size() < 3)
            continue;
        // search for single word - check all permutation of letters
        std::sort(letters.begin(), letters.end());
        do {
            if (std::binary_search(dict.begin(), dict.end(), letters))
                round_findings.emplace_back(letters, 0);
        } while (std::next_permutation(letters.begin(), letters.end()));
        // search for pair of words
        const int sz = int(letters.size());
        if (sz > 5) 
            for (int i = 0; i < sz; ++i)
                for (int j = i + 1; j < sz; ++j)
                    for (int k = j + 1; k < sz; ++k) {
                        std::string first, second;
                        std::vector<std::string> fres, sres;
                        for (int d = 0; d < sz; ++d)
                            if (d == i || d == j || d == k)
                                first.push_back(letters[d]);
                            else
                                second.push_back(letters[d]);
                        std::sort(first.begin(), first.end());
                        do {
                            if (std::binary_search(dict.begin(), dict.end(), first))
                                fres.push_back(first);
                        } while (std::next_permutation(first.begin(), first.end()));
                        if (fres.empty())
                            continue;
                        std::sort(second.begin(), second.end());
                        do {
                            if (std::binary_search(dict.begin(), dict.end(), second))
                                sres.push_back(second);
                        } while (std::next_permutation(second.begin(), second.end()));
                        if (sres.empty())
                            continue;
                        std::sort(fres.begin(), fres.end());
                        std::sort(sres.begin(), sres.end());
                        fres.erase(std::unique(fres.begin(), fres.end()), fres.end());
                        sres.erase(std::unique(sres.begin(), sres.end()), sres.end());
                        for (const auto& f : fres)
                            for (const auto& s : sres)
                                if (f < s)
                                    round_findings.emplace_back(f + s, f.size());
                                else
                                    round_findings.emplace_back(s + f, s.size());
                    }

        if (!round_findings.empty()) {
            if (best < ans) {
                best = ans;
                found = std::move(round_findings);
            } else {
                found.reserve(found.size() + round_findings.size());
                std::move(round_findings.begin(), round_findings.end(), std::back_inserter(found));
            }
        }
    }

    if (found.empty())
        task_out << "0\n";
    else {
        std::sort(found.begin(), found.end());
        found.erase(std::unique(found.begin(), found.end()), found.end());
        task_out << best << '\n';
        for (auto& f : found) {
            if (f.second > 0)
                f.first.insert(f.first.begin() + f.second, ' ');
            task_out << f.first << '\n';
        }
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 lgame.cpp -o lgame.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address lgame.cpp -o lgame

Input:

pofax

Output:

16
fox

*/

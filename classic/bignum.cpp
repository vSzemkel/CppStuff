
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <iterator>
#include <string>

// Positive big number
// Unsigned decimal base calculations
// DO NOT USE in production, use Python or this:
// https://github.com/michaeljclark/bignum

struct bignum_t {
    bignum_t(int64_t n = 0) {
        if (n == 0)
            _bignum.push_back(0);
        while (n) {
            _bignum.push_back(n % 10);
            n /= 10;
        }
    }

    bignum_t(const std::string& s) {
        assert(!s.empty());
        _bignum.resize(s.size());
        std::transform(s.crbegin(), s.crend(), _bignum.begin(), [](const char d){ return d - '0'; });
    }

    bool operator==(const bignum_t& other) const {
        return _bignum == other._bignum;
    }

    bool operator<(const bignum_t& other) const {
        const auto sz = _bignum.size(), osz = other._bignum.size();
        if (sz < osz)
            return true;
        if (sz > osz)
            return false;
        if (_bignum == other._bignum)
            return false;
        // not equal but equal size
        int pos = sz;
        while (_bignum[pos] == other._bignum[pos])
            --pos;
        return _bignum[pos] < other._bignum[pos];
    }

    auto to_string() const {
        if (_bignum.empty())
            return std::string{"0"};
        std::string ret;
        ret.reserve(_bignum.size());
        std::transform(_bignum.crbegin(), _bignum.crend(), std::back_inserter(ret), [](const char d){ return d + '0'; });
        return ret;
    }

    auto operator+(const bignum_t& other) const {
        const auto sz = int(std::min(_bignum.size(), other._bignum.size()));
        std::string ret;
        ret.reserve(sz);
        int carry{0};
        for (int i = 0; i < sz; ++i) {
            ret.push_back(_bignum[i] + other._bignum[i] + carry);
            auto& last = ret.back();
            carry = last / 10;
            last %= 10;
        }

        const auto& tail = (_bignum.size() < other._bignum.size()) ? other._bignum : _bignum;
        const auto tsz = int(tail.size());
        for (int i = sz; i < tsz; ++i) {
            if (carry == 0) {
                std::copy(tail.begin() + i, tail.end(), std::back_inserter(ret));
                break;
            }
            ret.push_back(tail[i] + carry);
            auto& last = ret.back();
            carry = last / 10;
            last %= 10;
        }

        if (carry)
            ret.push_back(carry);

        bignum_t wrapper;
        wrapper._bignum = std::move(ret);
        return wrapper;
    }

    void operator+=(const bignum_t& other) {
        int carry{0};
        const auto sz = int(std::min(_bignum.size(), other._bignum.size()));
        for (int i = 0; i < sz; ++i) {
            auto& cur = _bignum[i];
            const auto val = cur + other._bignum[i] + carry;
            cur = val % 10;
            carry = val / 10;
        }

        const auto& tail = (_bignum.size() < other._bignum.size()) ? other._bignum : _bignum;
        const auto tsz = int(tail.size());
        if (sz < tsz)
            _bignum.resize(tsz);
        for (int i = sz; i < tsz; ++i) {
            if (carry == 0)
                break;
            auto& cur = _bignum[i];
            const auto val = tail[i] + carry;
            cur = val % 10;
            carry = val / 10;
        }

        if (carry)
            _bignum.push_back(carry);
    }

    auto operator-(const bignum_t& other) const {
        if (_bignum == other._bignum)
            return bignum_t{};
        assert(other < *this);

        const auto sz = int(std::min(_bignum.size(), other._bignum.size()));
        std::string ret;
        ret.reserve(sz);
        int borrow{0};
        for (int i = 0; i < sz; ++i) {
            ret.push_back(_bignum[i] - other._bignum[i] - borrow);
            auto& last = ret.back();
            if (last < 0) {
                last += 10;
                borrow = 1;
            } else
                borrow = 0;
        }

        const auto& tail = (_bignum.size() < other._bignum.size()) ? other._bignum : _bignum;
        const auto tsz = int(tail.size());
        for (int i = sz; i < tsz; ++i) {
            if (borrow == 0) {
                std::copy(tail.begin() + i, tail.end(), std::back_inserter(ret));
                break;
            }
            ret.push_back(tail[i] - borrow);
            auto& last = ret.back();
            if (last < 0) {
                last += 10;
                borrow = 1;
            } else
                borrow = 0;
        }
        assert(borrow == 0);

        bignum_t wrapper;
        wrapper._bignum = std::move(ret);
        return wrapper;
    }

    bignum_t operator*(const bignum_t& other) const {
        const auto sz = _bignum.size(), osz = other._bignum.size();
        if (3 * sz < 2 * osz)
            return other * (*this);

        bignum_t ret{};
        ret._bignum.reserve(sz + osz);
        for (int i = 0; i < int(osz); ++i)
            ret = ret + digmul(other._bignum[i], i);

        return ret;
    }

    bignum_t operator^(bignum_t other) const {
        bignum_t base{*this}, ret{1}, zero{};
        while (zero < other) {
            if (other._bignum[0] & 1)
                ret = ret * base;
            base = base * base;
            other = other.digdiv(2);
        }

        return ret;
    }

  private:
    std::string _bignum;

    /**
     * @brief Multiply this by d and add p padding of least significant zeros to the result
     */
    bignum_t digmul(const char d, const int p) const {
        const auto sz = int(_bignum.size());
        std::string ret(p, 0);
        ret.reserve(sz);
        int carry{0};
        for (int i = 0; i < sz; ++i) {
            const auto v = _bignum[i] * d + carry;
            ret.push_back(v % 10);
            carry = v / 10;
        }
        if (carry > 0)
            ret.push_back(carry);

        bignum_t wrapper;
        wrapper._bignum = std::move(ret);
        return wrapper;
    }

    /**
     * @brief Divide this by d and discard the reminder
     */
    bignum_t digdiv(const char d) const {
        if (_bignum.empty())
            return bignum_t{};
        const auto sz = int(_bignum.size());
        std::string ret;
        ret.reserve(sz);
        int carry{0}, pos = sz - 1, msd = _bignum.back();
        if (msd < d) {
            carry = msd;
            --pos;
        }
        for (int i = pos; i >= 0; --i) {
            const auto cd = _bignum[i] + 10 * carry;
            ret.push_back(cd / d);
            carry = cd % d;
        }

        bignum_t wrapper;
        wrapper._bignum = std::move(ret);
        return wrapper; // reminder in carry
    }
};

int main(int, char**)
{
    assert(bignum_t{0} == bignum_t("0"));
    assert(bignum_t{1} == bignum_t("1"));
    std::string bs1 = "9992357899728851732500348514614308994657";
    std::string bs2 = "7642100271148267499651485385691005343";
    const bignum_t bns1{bs1};
    const bignum_t bns2{bs2};
    assert(bns1.to_string() == bs1);
    bignum_t bn{761009514385615};
    assert(bn.to_string() == "761009514385615");

    assert(bignum_t(1784) + bignum_t(306) == bignum_t(2090));
    assert(bns1 + bns2 == bignum_t("10000000000000000000000000000000000000000"));
    assert(bns1 - bns2 == bignum_t("9984715799457703465000697029228617989314"));

    assert((bignum_t(138) * bignum_t(7182)).to_string() == "991116");
    assert((bns1 * bns2).to_string().substr(0, 6) == "763626");

    assert((bignum_t(138) ^ bignum_t(5)).to_string() == "50049003168");
    assert((bignum_t(180) ^ bignum_t(92)).to_string().substr(0, 6) == "305541");

    assert(bignum_t("10000000009999999999") + 1 == bignum_t("10000000010000000000"));
    assert(bignum_t("10000000009999999999") == bignum_t("10000000010000000000") - 1);
    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 bignum.cpp -o bignum.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address bignum.cpp -o bignum

*/

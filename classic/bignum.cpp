
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <string>

// Bignum
// Unsigned decimal base calculations
// DO NOT USE in production, use Python or this:
// https://github.com/michaeljclark/bignum

struct bignum_t {
    bignum_t(int64_t n = 0) {
        while (n) {
            _bignum.push_back(n % 10);
            n /= 10;
        }
    }

    bignum_t(const std::string& s) {
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
        for (const auto d : _bignum)
            ret.push_back(d + '0');
        std::reverse(ret.begin(), ret.end());
        return ret;
    }

    auto operator+(const bignum_t& other) {
        const auto sz = int(std::min(_bignum.size(), other._bignum.size()));
        std::string ret;
        ret.reserve(sz);
        int carry = 0;
        for (int i = 0; i < sz; ++i) {
            ret.push_back(_bignum[i] + other._bignum[i] + carry);
            auto& last = ret.back();
            carry = last / 10;
            last %= 10;
        }

        const auto& tail = (_bignum.size() < other._bignum.size()) ? other._bignum : _bignum;
        const auto tsz = int(tail.size());
        for (int i = sz; i < tsz; ++i) {
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

    auto operator-(const bignum_t& other) {
        if (_bignum == other._bignum)
            return bignum_t{};
        assert(other < *this);

        const auto sz = int(std::min(_bignum.size(), other._bignum.size()));
        std::string ret;
        ret.reserve(sz);
        int borrow = 0;
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

  private:
    std::string _bignum;
};

int main(int, char**)
{
    std::string bs1 = "9992357899728851732500348514614308994657";
    std::string bs2 = "7642100271148267499651485385691005343";
    bignum_t bns{bs1};
    assert(bns.to_string() == bs1);
    bignum_t bn{761009514385615};
    assert(bn.to_string() == "761009514385615");

    assert(bignum_t(1784) + bignum_t(306) == bignum_t(2090));
    assert(bignum_t(bs1) + bignum_t(bs2) == bignum_t("10000000000000000000000000000000000000000"));
    assert(bignum_t(bs1) - bignum_t(bs2) == bignum_t("9984715799457703465000697029228617989314"));
    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 bignum.cpp -o bignum.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address bignum.cpp -o bignum

Run:
py.exe interactive_runner.py py.exe bignum_testing_tool.py 1 -- bignum.exe
bignum.exe < bignum.in

Input:


Output:


*/

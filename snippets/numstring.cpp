#include <cassert>
#include <iostream>
#include <string>
#include <random>

static constexpr const int L = 8;
static constexpr const int BASE = 2;
static constexpr const char ZERO = '0';
const auto rand_in_range = [](const int64_t ubound){std::random_device seed;std::mt19937 gen{seed()};std::uniform_int_distribution<int64_t> dist(0,ubound-1);return dist(gen);};

template <typename T = int>
static std::string binstr(const T val) { // 5 -> "00000101", when L == 8
    std::string ret(L, '0');
    for (T p = 0, m = 1 << (L - 1); m; m >>= 1, ++p)
        if (val & m) ret[p] = '1';
    return ret;
}

template <typename T = int>
static T numbin(std::string str) { // "00000101" -> 5, when L == 8
    T ret{0};
    for (int i = 0; i < L; ++i)
        ret = (ret << 1) + str[i] - ZERO;
    return ret;
}

static std::string rndnumstr() {
    std::string ret(L, ZERO);
    for (int p = 0; p < L; ++p)
        ret[p] = char(rand_in_range(BASE)) + ZERO;
    return ret;
}

static std::string add_one(const std::string& num)
{
    auto ret = num;
    for (int d = (int)ret.size() - 1; d >= 0; --d)
        if (ret[d] < '9') {
            ++ret[d];
            return ret;
        } else 
            ret[d] = '0';

    ret.insert(ret.begin(), '1');
    return ret;
}

static std::string minus_one(const std::string& num)
{
    auto ret = num;
    for (int d = (int)ret.size() - 1; d >= 0; --d)
        if (ret[d] > '0') {
            --ret[d];
            break;
        } else 
            ret[d] = '9';

    if (ret[0] == '0' && ret.size() > 1)
        ret.erase(ret.begin());

    return ret;
}

int main(int, char**)
{
    int n{0};
    std::string s{"0"};
    for (int i = 0; i < 256; ++i) {
        const auto bs = binstr(i);
        const auto val = numbin(bs);
        assert(val == i);
    }
    for (int i = 0; i < 100000; ++i) {
        ++n;
        s = add_one(s);
        assert(s == std::to_string(n));
    }
    for (int i = 0; i < 100000; ++i) {
        --n;
        s = minus_one(s);
        assert(s == std::to_string(n));
    }

    assert(rndnumstr().size() == L);
    std::cout << "PASSED\n";
}

// clang++.exe -Wall -Wextra -g -O0 -std=c++17 numstring.cpp -o numstring.exe
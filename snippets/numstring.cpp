#include <cassert>
#include <string>

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
}

// clang++.exe -Wall -Wextra -g -O0 -std=c++17 numstring.cpp -o numstring.exe
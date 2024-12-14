/*
ID: marcin.8
LANG: C++
TASK: latin
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=q5IyqPflpNO&S=latin
*/

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <filesystem>
#include <fstream>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <vector>

std::ifstream task_in("latin.in");
std::ofstream task_out("latin.out");

template<int N>
class positional_number_t
{
  public:
    positional_number_t(const int value = 0) : _value(value) {
        syncn2d();
    }

    positional_number_t(const std::vector<int>& digits) {
        assert(digits.size() <= N);
        int i = int(digits.size());
        for (const int d : digits)
            _digits[--i] = d;
        sync();
    }

    positional_number_t(const std::vector<int>& digits) {
        assert(digits.size() <= N);
        int i = int(digits.size());
        for (const int d : digits)
            _digits[--i] = d;
        sync();
    }

    positional_number_t(const std::initializer_list<int> digits) {
        assert(digits.size() <= N);
        int i = int(digits.size());
        for (const int d : digits)
            _digits[--i] = d;
        sync();
    }

    positional_number_t& operator=(const int value) {
        _value = value;
        syncn2d();
        return *this;
    }

    operator int() const {
        return _value;
    }

    int operator[](const int index) const {
        return _digits[index];
    }

    int& operator[](const int index) {
        return _digits[index];
    }

    int reverse() const {
        int ret{};
        for (const int& d : _digits)
            ret = 10 * ret + d;

        return ret;
    }

    int push_back(const int digit) {
        assert(0 <= digit && digit < 10);
        _value = 10 * _value + digit;
        for (int i = N - 1; i > 0; --i)
            _digits[i] = _digits[i - 1];
        _digits[0] = digit;
        return _value;
    }

    int pop_back(int count) {
        assert(0 <= count && count <= N);
        for (int i = count; i < N; ++i)
            _digits[i - count] = _digits[i];
        for (int i = N - 1; count; --i, --count) {
            _digits[i] = 0;
            _value /= 10;
        }
        return _value;
    }

    void sync() {
        _value = 0;
        for (int i = N - 1; ~i; --i)
            _value = 10 * _value + _digits[i];
    }

  private:
    void syncn2d() {
        auto tmp = _value;
        for (int& d : _digits) {
            d = tmp % 10;
            tmp /= 10;
        }
    }

    int _value{};
    int _digits[N] = {};
};

int N, ans{};
using num_t = positional_number_t<7>;
std::unordered_set<int> prefixes;
std::vector<num_t> numbers;

void generate() {
    std::vector<int> digits(N);
    std::iota(digits.begin(), digits.end(), 1);
    do {
        num_t num;
        numbers.emplace_back(digits);
    } while (std::next_permutation(digits.begin(), digits.end()));
}

int main(int, char**)
{
    std::vector<int> test = {4, 2, 0, 1};
    num_t x{test};

    task_in >> N;
    task_out << ans << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 latin.cpp -o latin.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address latin.cpp -o latin

Run:
latin.exe && type latin.out

Input:

5

Output:

1344

*/

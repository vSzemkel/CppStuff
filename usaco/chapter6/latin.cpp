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

int N, epilog, ans{};
using num_t = positional_number_t<7>;
std::unordered_set<int> prefixes;
std::vector<num_t> numbers, rows, cols;

int generate() {
    std::vector<int> digits(N);
    std::iota(digits.begin(), digits.end(), 1);
    do {
        num_t num;
        int d = numbers.emplace_back(digits);
        while (d) {
            prefixes.insert(d);
            d /= 10;
        }
    } while (std::next_permutation(digits.begin(), digits.end()));

    return numbers.size();
}

auto prefix_range(const num_t& prefix)
{
    assert(prefix > 0);
    auto low = prefix;
    while (low[N - 1] == 0)
        low.push_back(0);
    auto high = prefix;
    while (high[N - 1] == 0)
        high.push_back(9);

    const auto beg = std::lower_bound(numbers.begin(), numbers.end(), low);
    return std::make_pair(beg, std::lower_bound(beg, numbers.end(), high));
}

void inner_solve(const int round)
{
    if (round == epilog) {
        ++ans;
        return;
    }

    int cur = round / 2;
    if (round & 1) { // fill column
        const auto old_col = cols[cur];
        const auto [low, high] = prefix_range(old_col);
        for (auto it = low; it != high; ++it) {
            bool valid{true};
            for (int r = cur + 1; r < N; ++r)
                if (!prefixes.count(rows[r] * 10 + (*it)[N - 1 - r]))
                    valid = false;

            if (!valid)
                continue;

            cols[cur] = *it;
            for (int r = cur + 1; r < N; ++r)
                rows[r].push_back((*it)[N - 1 - r]);
            inner_solve(round + 1);
            for (int r = cur + 1; r < N; ++r)
                rows[r].pop_back(1);
            cols[cur] = old_col;
        }
    } else { // fill row
        const auto old_row = rows[cur];
        const auto [low, high] = prefix_range(old_row);
        for (auto it = low; it != high; ++it) {
            bool valid{true};
            for (int c = cur; c < N; ++c)
                if (!prefixes.count(cols[c] * 10 + (*it)[N - 1 - c]))
                    valid = false;

            if (!valid)
                continue;

            rows[cur] = *it;
            for (int c = cur; c < N; ++c)
                cols[c].push_back((*it)[N - 1 - c]);
            inner_solve(round + 1);
            for (int c = cur; c < N; ++c)
                cols[c].pop_back(1);
            rows[cur] = old_row;
        }
    }
}

int main(int, char**)
{
    task_in >> N;
    rows.resize(N);
    cols.resize(N);
    epilog = 2 * N - 2;
    const int sz = generate();

    /*for (int i = 0; i < sz; ++i) {
        rows[0] = numbers[i];*/
        rows[0] = numbers[0];
        for (int c = 1; c < N; ++c)
            cols[c] = rows[0][N - 1 - c];

        const auto [low, high] = prefix_range(rows[0][N - 1]);
        for (auto it = low; it != high; ++it) {
            cols[0] = *it;
            for (int r = 1; r < N; ++r)
                rows[r] = cols[0][N - 1 - r];
            inner_solve(2);
        }
    //}

    task_out << ans << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++20 latin.cpp -o latin.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address latin.cpp -o latin

Run:
latin.exe && type latin.out

Input:

5

Output:

1344

*/

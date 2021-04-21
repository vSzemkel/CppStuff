#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

constexpr int length = 18;
constexpr int size = 1000;
std::vector<int64_t> factor10;

const auto rand_in_range = [](const int64_t ubound){std::random_device seed;std::mt19937 gen{seed()};std::uniform_int_distribution<int64_t> dist(0,ubound-1);return dist(gen);};

static std::vector<int64_t> counting_sort(const std::vector<int64_t>& data, const int pos) {
    const int sz = (int)data.size();
    assert(0 <= pos && pos < sz);

    std::vector<int> counts(10, 0);
    const auto digit_factor = factor10[pos];
    for (const auto d : data)
        ++counts[d / digit_factor % 10];

    std::vector<int64_t> ret(sz);
    std::inclusive_scan(counts.begin(), counts.end(), counts.begin());
    for (auto it = data.rbegin(); it != data.rend(); ++it) {
        const int p = --counts[*it / digit_factor % 10];
        ret[p] = *it;
    }

    return ret;
}

int main(int, char**) {
    std::vector<int64_t> data(size);
    const int64_t maxval = (int64_t)std::pow(10, length);
    for (auto& d : data)
        d = rand_in_range(maxval);

    auto copy = data;
    factor10.resize(length);
    for (int i = 0; i < length; ++i) {
        factor10[i] = (i == 0) ? 1 : 10 * factor10[i - 1];
        data = counting_sort(data, i);
    }

    std::sort(copy.begin(), copy.end());
    assert(data == copy);

    for (const auto n : data)
        std::cout << n << '\n';
}

/*
clang++.exe -Wall -Wextra -g -O0 -std=c++17 radix_sort.cpp -o radix_sort.exe
*/
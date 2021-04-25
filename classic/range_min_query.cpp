
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

// Range minimum query
// https://en.wikipedia.org/wiki/Range_minimum_query


template<typename T, bool maximum_mode = false> // inspired by neal_wu
struct RMQ {
    RMQ(const std::vector<T>& v) : _size((int)v.size()) {
        if (_size > 0) {
            _values = v;
            build();
        }
    }

    static int highest_bit(const int x) {
        return x == 0 ? -1 : 31 - __builtin_clz(x);
    }

    // Note: breaks ties by choosing the largest index.
    int query_index(const int a, const int b) const {
        assert(0 <= a && a < b && b <= _size);
        const int level = highest_bit(b - a);
        const auto& row = _range_low[level];
        return better_index(row[a], row[b - (1 << level)]);
    }

    T query_value(const int a, const int b) const {
        return _values[query_index(a, b)];
    }

  private:
    void build() {
        const int levels = highest_bit(_size) + 1;

        _range_low.resize(levels);
        for (int k = 0; k < levels; ++k)
            _range_low[k].resize(_size - (1 << k) + 1);

        auto& prev = _range_low[0];
        std::iota(prev.begin(), prev.end(), 0);
        for (int k = 1; k < levels; ++k) {
            auto& curr = _range_low[k];
            for (int i = 0; i <= _size - (1 << k); ++i)
                curr[i] = better_index(prev[i], prev[i + (1 << (k - 1))]);
            prev = curr;
        }
    }

    // Note: when `values[a] == values[b]`, returns b.
    int better_index(const int a, const int b) const {
        return (maximum_mode ? _values[b] < _values[a] : _values[a] < _values[b]) ? a : b;
    }

    int _size{0};
    std::vector<T> _values;
    std::vector<std::vector<int>> _range_low;
};


constexpr int trials = 100;
constexpr int size = 10000;
constexpr int range = 1'000'000'000;
const auto rand_in_range = [](const int ubound){ std::random_device seed; std::mt19937 gen{seed()}; std::uniform_int_distribution<int> dist(0, ubound - 1); return dist(gen); };

int main(int, char**)
{
    std::vector<int> data;
    data.resize(size);
    std::generate_n(std::back_inserter(data), size, []{ return rand_in_range(range); });

    RMQ<int, true> rmq{data};
    for (int i = 0; i < trials; ++i) {
        int lb = rand_in_range(range);
        int rb = rand_in_range(range);
        if (lb == rb) continue;
        if (lb > rb) std::swap(lb, rb);

        const int naive = *std::max(data.begin() + lb, data.begin() + rb + 1);
        const int better = rmq.query_value(lb, rb);
        assert(better == naive);
    }
}

/* Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 range_min_query.cpp -o range_min_query.exe
*/

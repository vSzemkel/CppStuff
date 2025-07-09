
#include <algorithm>
#include <cassert>
#include <format>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

// Subrange updates range query for huge ranges (Fenwick tree)
// Idea: Represent range values as prefix sums in first Binary Index Tree
// Idea: Represent range updates as differences between prefix updates in second BIT
// https://www.geeksforgeeks.org/dsa/binary-indexed-tree-range-update-range-queries/

template <typename U = int>
struct range_updates_range_queries_t
{
  private:
    template <typename T>
    struct subrange_sum_t
    {
        subrange_sum_t(const int size = 0) : _size(size), _data(size)
        {
        }

        subrange_sum_t(const std::vector<T>& data)
        {
            reset(data);
        }

        void reset(const std::vector<T>& data)
        {
            _size = data.size();
            _data.assign(_size, T{});
            for (int i = 0; i < _size; ++i)
                for (int a = i; a < _size; a |= a + 1)
                    _data[a] += data[i];
        }

        void update(const int pos, const T delta)
        {
            for (auto i = pos; i < _size; i |= i + 1)
                _data[i] += delta;
        }

        T query(const int pos)
        {
            return query(0, pos);
        }

        T query(const int i, const int j)
        { // closed range [i..j]
            T ret{0};
            for (int k = j + 1; k > 0; k &= k - 1)
                ret += _data[k - 1];
            for (int k = i; k > 0; k &= k - 1)
                ret -= _data[k - 1];

            return ret;
        }

        int _size;
        std::vector<T> _data;
    };

    subrange_sum_t<U> _ss0; // used only for initially initialized data
    subrange_sum_t<U> _ss1; // used for point queries
    subrange_sum_t<U> _ss2; // used for overcounting fixes
    bool _has_init_value;

    U sum(const int pos)
    {
        return _ss1.query(0, pos) * pos - _ss2.query(0, pos);
    }

  public:
    range_updates_range_queries_t(const int size = 0)
        : _ss1(size)
        , _ss2(size)
        , _has_init_value(false)
    {
    }

    range_updates_range_queries_t(const std::vector<U>& data)
        : _ss0(data)
        , _ss1(data.size())
        , _ss2(data.size())
        , _has_init_value(true)
    {
        reset(data);
    }

    void reset(const std::vector<U>& data)
    {
        const auto size = data.size();
        _ss0 = data;
        _ss1.reset(std::vector<U>(size));
        _ss2 = _ss1;
        _has_init_value = true;
    }
    
    void update(const int lo, const int hi, const U delta)
    {
        _ss1.update(lo, delta);
        _ss1.update(hi + 1, -delta);
        _ss2.update(lo, (lo - 1) * delta);
        _ss2.update(hi + 1, -hi * delta);
    }

    U query(const int lo, const int hi)
    {
        auto ret = sum(hi);
        if (lo > 0)
            ret -= sum(lo - 1);
        if (_has_init_value)
            ret += _ss0.query(lo, hi);
        return ret;
    }
};

std::vector<int64_t> g_benchmark;
range_updates_range_queries_t<int64_t> g_su;
const auto rand_in_range = [](const int ubound) { std::random_device seed; std::mt19937 gen{seed()}; std::uniform_int_distribution<int> dist(0, ubound - 1); return dist(gen); };

template <typename C>
static void print(const C& v)
{
    if (v.empty()) return;
    std::cout << v.front();
    const int size = int(v.size());
    for (int i = 1; i < size; ++i)
        std::cout << ' ' << v[i];
    std::cout << '\n';
}

void update(const int lo, const int hi, const int delta)
{
    for (int i = lo; i <= hi; ++i)
        g_benchmark[i] += delta;
    g_su.update(lo, hi, delta);
}

void check()
{
    // print(g_benchmark);

    const int size = g_benchmark.size();
    for (int l = 0; l < size; ++l)
        for (int u = l; u < size; ++u) {
            const auto acc = std::accumulate(g_benchmark.begin() + l, g_benchmark.begin() + u + 1, int64_t{});
            if (g_su.query(l, u) != acc)
                std::cout << std::format("Mismatch at range [{}, {}]. Expected {}, actual {}\n", l, u, acc, g_su.query(l, u));
        }
}

int main(int argc, char* argv[])
{
    constexpr int size = 300;
    constexpr int range = 1000000;

    std::generate_n(std::back_inserter(g_benchmark), size, [] { return rand_in_range(range); });
    g_su.reset(g_benchmark);

    for (int z = 100; z; --z) {
        const int delta = rand_in_range(range / 10);
        int lo = rand_in_range(size);
        int hi = rand_in_range(size);
        if (hi < lo)
            std::swap(lo, hi);

        update(lo, hi, delta);
        check();
    }

    std::cout << "PASSED\n";
}

/*
clang++.exe -Wall -ggdb3 -O0 -std=c++20 subrange_update_range_query.cpp
g++ -Wall -ggdb3 -Og -std=c++20 subrange_update_range_query.cpp
*/

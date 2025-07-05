
#include <algorithm>
#include <cassert>
#include <format>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

// Subrange updates point query for huge ranges (Fenwick tree)
// Idea: Represent range values as prefix sums in Binary Index Tree
// https://www.geeksforgeeks.org/dsa/binary-indexed-tree-range-updates-point-queries/

template <typename U = int>
struct subrange_updates_t
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

    subrange_sum_t<U> _ss;

  public:
    subrange_updates_t(const int size = 0) : _ss(size)
    {
    }

    subrange_updates_t(const std::vector<U>& data)
    {
        reset(data);
    }

    void reset(std::vector<U> data)
    {
        // transform data, so old_data[i] == prefsum(new_data, i)
        int acc{};
        for (auto& d : data) {
            d -= acc;
            acc += d;
        }

        _ss.reset(data);
    }

    void update(const int lo, const int hi, const U delta)
    {
        _ss.update(lo, delta);
        _ss.update(hi + 1, -delta);
    }

    U query(const int pos)
    {
        return _ss.query(0, pos);
    }
};

subrange_updates_t<int64_t> g_su;
std::vector<int64_t> g_benchmark;
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
    for (int m = 0; m < size; ++m)
        if (g_su.query(m) != g_benchmark[m])
            std::cout << std::format("Mismatch at position {}. Expected {}, actual {}\n", m, g_benchmark[m], g_su.query(m));
}

int main(int argc, char* argv[])
{
    constexpr int size = 1000;
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
clang++.exe -Wall -ggdb3 -O0 -std=c++20 subrange_update_point_query.cpp
g++ -Wall -ggdb3 -Og -std=c++20 subrange_update_point_query.cpp
*/

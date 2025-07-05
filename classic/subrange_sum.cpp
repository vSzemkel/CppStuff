
#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

// Prefix sum operations for huge ranges (Fenwick tree)
// Inspired by ecnervala task solutions on Google coding competitions

template <typename T = int>
struct subrange_sum_t
{
    subrange_sum_t(const int size = 0) : _size(size), _data(size) {
    }

    subrange_sum_t(const std::vector<T>& data) {
        reset(data);
    }

    void reset(const std::vector<T>& data) {
        _size = data.size();
        _data.assign(_size, T{});
        for (int i = 0; i < _size; ++i) 
            for (int a = i; a < _size; a |= a + 1)
                _data[a] += data[i];
    }

    void update(const int pos, const T delta) {
        for (auto i = pos; i < _size; i |= i + 1)
            _data[i] += delta;
    }

    T query(const int pos) {
        return query(0, pos);
    }

    T query(const int i, const int j) { // closed range [i..j]
        T ret{0};
        for (int k = j + 1; k > 0; k &= k - 1)
            ret += _data[k - 1];
        for (int k = i; k > 0; k &= k - 1)
            ret -= _data[k - 1];

        return ret;
    }

    int ksum_prefix(const T value) {
        int l{0}, r{_size};
        while (l < r) {
            const auto mid = l + (r - l) / 2;
            if (query(mid) >= value)
                r = mid;
            else
                l = mid + 1;
        }
        return l; // l = min(i) : query(0, i) >= value || l == _size if total < value
    }

    int ksum_sufix(const T value) {
        int l{-1}, r{_size - 1}, total = query(r);
        while (l < r) {
            const auto mid = l + (r - l + 1) / 2;
            if (query(mid) <= total - value)
                l = mid;
            else
                r = mid - 1;
        }
        if (total >= value && l < _size - 1) ++l;
        return l; // l = max(i) : query(i, ub) >= value || l == -1 if total < value
    }

    int _size;
    std::vector<T> _data;
};

subrange_sum_t<int64_t> g_ss;
std::vector<int64_t> g_input, g_partial, g_ecnerwala;

template <typename T> void incl_scan(const T& src) { typename T::value_type acc{}; g_partial = {acc}; for (const auto& v : src) { acc += v; g_partial.push_back(acc); };};
int64_t partial_sum(const int i, const int j) { return g_partial[j + 1] - g_partial[i]; }
const auto rand_in_range = [](const int ubound){ std::random_device seed; std::mt19937 gen{seed()}; std::uniform_int_distribution<int> dist(0, ubound - 1); return dist(gen); };

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

void ecnervala_init(const std::vector<int64_t>& src)
{
    const int size = src.size();
    g_ecnerwala.assign(size, 0);
    for (int i = 0; i < size; ++i) 
        for (int a = i; a < size; a |= a + 1)
            g_ecnerwala[a] += src[i];
}

void ecnervala_update(std::vector<int64_t>& src, const size_t pos, const int64_t val)
{
    const auto size = src.size();
    for (auto i = pos; i < size; i |= i + 1)
        g_ecnerwala[i] += val - src[pos];
    src[pos] = val;
}

int64_t ecnerwala_partial_sum(const int i, const int j)
{
    int64_t ret{0};
    for (int k = j + 1; k > 0; k &= k - 1)
        ret += g_ecnerwala[k - 1];
    for (int k = i; k > 0; k &= k - 1)
        ret -= g_ecnerwala[k - 1];

    return ret;
}

void check()
{
    print(g_input);
    print(g_partial);
    print(g_ecnerwala);
    std::cout << "\n";
    
    const int size = g_input.size();
    for (int m = 0; m < size - 1; ++m)
        for (int n = m + 1; n < size; ++n) {
            if (partial_sum(m, n) != ecnerwala_partial_sum(m, n))
                std::cout << "Error1: " << m << " " << n << " " << partial_sum(m, n) << " " << ecnerwala_partial_sum(m, n) << "\n";
            if (partial_sum(m, n) != g_ss.query(m, n))
                std::cout << "Error2: " << m << " " << n << " " << partial_sum(m, n) << " " << g_ss.query(m, n) << "\n";
        }
}

int main(int argc, char* argv[])
{
    constexpr int size = 20;
    constexpr int range = 1000000;

    //g_input.assign(size, 1);
    std::generate_n(std::back_inserter(g_input), size, []{ return 2 + rand_in_range(range); });

    incl_scan(g_input);
    ecnervala_init(g_input);
    g_ss.reset(g_input);
    check();

    // set weights of elements 5 and size - 7 to val
    const int64_t val = 17;
    for (int i = 5 + 1; i <= size; ++i) g_partial[i] += val - g_input[5];
    for (int i = size - 7 + 1; i <= size; ++i) g_partial[i] += val - g_input[size - 7];
    g_ss.update(5, val - g_input[5]);
    g_ss.update(size - 7, val - g_input[size - 7]);
    ecnervala_update(g_input, 5, 17); // clobbers g_input
    ecnervala_update(g_input, size - 7, 17);
    check();

    // Fenwick tree ksum operations
    const auto total = g_ss.query(size - 1);
    auto exactsum = g_ss.query(7);
    int preflen = g_ss.ksum_prefix(exactsum);
    int suflen = g_ss.ksum_sufix(total - exactsum);
    assert(0 < preflen && preflen + 1 == suflen && suflen < size);
    ++exactsum; // {g_input[i] >= 2}
    int preflen2 = g_ss.ksum_prefix(exactsum);
    int suflen2 = g_ss.ksum_sufix(total - exactsum);
    assert(preflen + 1 == preflen2 && preflen2 == suflen2);
    assert(g_ss.ksum_prefix(0) == 0);
    assert(g_ss.ksum_sufix(0) == size - 1);
    assert(g_ss.ksum_prefix(total) == size - 1);
    assert(g_ss.ksum_sufix(total) == 0);
    assert(g_ss.ksum_prefix(2 * total) == size);
    assert(g_ss.ksum_sufix(2 * total) == -1);

    // Fenwick tree as Order Statistics Tree
    // REQ: domain is constraint as we need to allocate array[N]
    // For general approach with rb_tree see:
    // https://github.com/kth-competitive-programming/kactl/blob/main/content/data-structures/OrderStatisticTree.h
    const int MAX = 10000;
    std::vector<int> os_sample = {2, 62, 172, 671, 1200, 4981, 9999};
    std::vector<int> os_probe(MAX + 1);
    for (const int s : os_sample)
        os_probe[s] = 1;
    subrange_sum_t ost{os_probe};
    // rank(1200) == 5
    assert(ost.query(1200) == 5);
    // order(5) == 1200
    assert(ost.ksum_prefix(5) == 1200);
    // delete 671, insert 1300
    ost.update(671, -1);
    ost.update(1300, 1);
    assert(ost.query(1200) == 4);
    assert(ost.ksum_prefix(5) == 1300);
    std::cout << "PASSED\n";
}

/*
clang++.exe -Wall -ggdb3 -O0 -std=c++17 subrange_sum.cpp -o subrange_sum.exe
g++ -Wall -ggdb3 -Og -std=c++17 subrange_sum.cpp -o subrange_sum.o
*/


#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

// Prefix sum operations for huge ranges
// Inspired by encervala task solutions on Google coding competitions

template <typename T> using vec = std::vector<T>;
vec<int64_t> g_input, g_partial, g_ecnerwala;

template <typename T> void incl_scan(const T& src) { typename T::value_type acc{}; g_partial = {acc}; for (const auto& v : src) { acc += v; g_partial.push_back(acc); };};
int64_t partial_sum(const int i, const int j) { return g_partial[j + 1] - g_partial[i]; }
const auto rand_in_range = [](const int ubound){ std::random_device seed; std::mt19937 gen{seed()}; std::uniform_int_distribution<int> dist(0, ubound - 1); return dist(gen); };

void print(const std::vector<int64_t>& v)
{
    for (const auto& n : v) std::cout << n << " ";
    std::cout << "\n";
}

void encervala_init(const std::vector<int64_t>& src)
{
    const int size = src.size();
    g_ecnerwala.assign(size, 0);
    for (int i = 0; i < size; ++i) 
        for (int a = i; a < size; a |= a + 1)
            g_ecnerwala[a] += src[i];
}

void encervala_update(std::vector<int64_t>& src, const size_t pos, const int64_t val)
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
        for (int n = m + 1; n < size; ++n)
            if (partial_sum(m, n) != ecnerwala_partial_sum(m, n))
                std::cout << "Error: " << m << " " << n << " " << partial_sum(m, n) << " " << ecnerwala_partial_sum(m, n) << "\n";
}

int main(int argc, char* argv[])
{
    constexpr int size = 20;
    constexpr int range = 1000000;

    //g_input.assign(size, 1);
    std::generate_n(std::back_inserter(g_input), size, []{ return rand_in_range(range); });

    incl_scan(g_input);
    encervala_init(g_input);
    check();

    // set weights of elements 5 and size - 7 to val
    const int val = 17;
    for (int i = 5 + 1; i <= size; ++i) g_partial[i] += val - g_input[5];
    for (int i = size - 7 + 1; i <= size; ++i) g_partial[i] += val - g_input[size - 7];
    encervala_update(g_input, 5, 17);
    encervala_update(g_input, size - 7, 17);
    check();
}

/*
clang++.exe -Wall -ggdb3 -O0 -std=c++17 prefix_sum.cpp -o prefix_sum.exe
g++ -Wall -ggdb3 -Og -std=c++17 prefix_sum.cpp -o prefix_sum.o
*/

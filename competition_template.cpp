
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <fstream>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <random>
#include <set>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

const auto big64 = std::numeric_limits<int64_t>::max();
template <typename T> using vec = std::vector<T>;
template <typename T> using vec2 = std::vector<std::vector<T>>;
vec<int64_t> g_input, g_partial;

template <typename T> void fill(T& cont, const int size){ cont.resize(size), std::copy_n(std::istream_iterator<typename T::value_type>{std::cin}, size, cont.begin()); };
template <typename T> int fill(T& cont){ int size; std::cin >> size; fill(cont, size); return size; };
template <typename T> int fill2(T& cont){ int rows, cols, size; std::cin >> rows >> cols; size = rows * cols;  cont.resize(rows); for (auto& r : cont) fill(r, cols); return size; };
template <typename T> T init2(const int rows, const int cols){ T cont(rows, typename T::value_type(cols)); return cont; };
template <typename T> void incl_scan(const T& src, T& dst) {T::value_type s{}; dst = {}; for (const auto& n : src) { s += n; dst.push_back(s); };}
int64_t partial_sum(const int i, const int j) { return g_partial[j + 1] - g_partial[i]; }
ptrdiff_t next_max(const int off) { return std::lower_bound(g_input.begin() + off + 1, g_input.end(), 0, [](const auto& n, auto) { const auto pred = &n - 1; return *pred <= n; }) - g_input.begin() - 1;}
ptrdiff_t next_min(const int off) { return std::lower_bound(g_input.begin() + off + 1, g_input.end(), 0, [](const auto& n, auto) { const auto pred = &n - 1; return *pred >= n; }) - g_input.begin() - 1;}

int64_t solve() {
    return big64;
}

int main(int argc, char* argv[])
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    // parse console input
    int no_of_cases, size;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        size = fill(g_input);
        // Set 1
        std::cout << "Case #" << g << ": " << std::setprecision(15) << solve() << "\n";
    }
}

/*
clang++.exe -Wall -ggdb3 -O0 -std=c++17 $TASKNAME$.cpp -o $TASKNAME$.exe
$TASKNAME$.exe < $TASKNAME$.in

Input:


Output:


*/
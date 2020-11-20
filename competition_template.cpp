
#include <algorithm>
#include <array>
#include <assert.h>
#include <cmath>
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
#include <queue>
#include <random>
#include <set>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// $TASKNAME$
// https://

const auto big64 = std::numeric_limits<int64_t>::max();
template <typename T> using vec = std::vector<T>;
template <typename T> using vec2 = std::vector<std::vector<T>>;
vec<int64_t> g_input, g_partial;
vec<bool> g_marked;
size_t g_size;

template <typename T> std::vector<T> fill(const size_t size){ std::vector<T> cont(size); std::copy_n(std::istream_iterator<T>{std::cin}, size, cont.begin()); return cont; };
template <typename T> std::vector<std::vector<T>> fill2(T& cont){ int rows, cols; std::cin >> rows >> cols; cont.resize(rows); for (auto& r : cont) r = fill<T>(cols); return cont; };
template <typename K, typename V> std::map<K, V> fillmap(size_t size){ std::map<K, V> cont; for (int i = 0; i < size; ++i) { int k, v; std::cin >> k >> v; cont[k] = v;} return cont; };
template <typename T> T init2(const int rows, const int cols){ T cont(rows, typename T::value_type(cols)); return cont; };
template <typename T> void incl_scan(const T& src, T& dst) {typename T::value_type s{}; dst = {}; for (const auto& n : src) { s += n; dst.push_back(s); };}
const auto rand_in_range = [](const int ubound){ std::random_device seed; std::mt19937 gen{seed()}; std::uniform_int_distribution<int> dist(0, ubound - 1); return dist(gen); };
int64_t partial_sum(const int i, const int j) { return g_partial[j + 1] - g_partial[i]; }
size_t next_max(const int off) { return std::lower_bound(g_input.begin() + off + 1, g_input.end(), 0, [](const auto& n, auto) { const auto pred = &n - 1; return *pred <= n; }) - g_input.begin() - 1;}
size_t next_min(const int off) { return std::lower_bound(g_input.begin() + off + 1, g_input.end(), 0, [](const auto& n, auto) { const auto pred = &n - 1; return *pred >= n; }) - g_input.begin() - 1;}

int64_t solve() {
    return big64;
}

int main(int argc, char* argv[])
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    // parse console input
    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        // Set 1
        std::cout << "Case #" << g << ": " << std::setprecision(15) << solve() << "\n";
    }
}

/*
clang++.exe -Wall -ggdb3 -O0 -std=c++17 $TASKNAME$.cpp -o $TASKNAME$.exe
g++ -Wall -ggdb3 -O0 -std=c++17 $TASKNAME$.cpp -o $TASKNAME$.o
$TASKNAME$.exe < $TASKNAME$.in

Input:


Output:


*/

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <format>
#include <memory>
#include <numeric>
#include <random>
#include <span>
#include <vector>

// Wavelet Tree
// Subrange query series of k-th largest element

struct wavelet_tree_t
{
    wavelet_tree_t(const std::span<int> input, int min_value = -1, int max_value = -1)
    {
        if (min_value == -1 && max_value == -1) {
            const auto [m, M] = std::minmax_element(input.begin(), input.end());
            _min_value = *m;
            _max_value = *M;
        } else {
            _min_value = min_value;
            _max_value = max_value;
        }

        if (_min_value < _max_value)
            build(input);
    }

    // Find k-th largest in subrange [lo..hi] of values in range [min_value..max_value]
    int kth(int lo, int hi, int k)
    {
        if (hi < lo)
            return -1;
        if (_min_value == _max_value)
            return _min_value;

        const int ll = _next_left[lo - 1];
        const int hh = _next_left[hi];
        const int left_size = hh - ll;
        if (0 < left_size && k <= left_size)
            return _left->kth(ll + 1, hh, k);

        // lo - 1 - ll : this many out of first lo-1 numbers goes to the right
        // hi - hh : this many out of first hi numbers goes to the right
        return _right->kth(lo - ll, hi - hh, k - left_size);
    }

  private:
    void build(const std::span<int> input)
    {
        std::vector<int> child[2];
        _next_left.reserve(input.size() + 1);
        _next_left.push_back(0);
        const int mid = _min_value + (_max_value - _min_value) / 2;
        for (const int v : input) {
            child[mid < v].push_back(v);
            _next_left.push_back(child[0].size());
        }

        _left = std::make_unique<wavelet_tree_t>(child[0], _min_value, mid);
        _right = std::make_unique<wavelet_tree_t>(child[1], mid + 1, _max_value);
    }

    int _min_value, _max_value;
    std::vector<int> _next_left;
    std::unique_ptr<wavelet_tree_t> _left, _right;
};

constinit const int size = 1000;
static std::mt19937 _gen{std::random_device{}()};
const auto rand_in_range = [](const int ubound){ std::uniform_int_distribution<int> dist(0, ubound - 1); return dist(_gen); };

int main(int, char**)
{
    std::array<int, size> a{};
    std::iota(a.begin(), a.end(), 1);
    std::shuffle(a.begin(), a.end(), _gen);

    int m = rand_in_range(size);
    int M = rand_in_range(size) + 1;
    if (M < m) std::swap(m, M);
    int k = rand_in_range(M - m + 1);


m = 100; M = 101; k = 0;

    std::vector<int> subrange{a.begin() + m, a.begin() + M + 1};
    std::sort(subrange.begin(), subrange.end());

    wavelet_tree_t wt(a);
    const int kth = wt.kth(m, M, k);
    std::cout << std::format("Bruteforce: {}, wavelet: {}\n", subrange[k], kth);
    assert(subrange[k] == kth);
    std::cout << "Passed\n";
}
/*

Format:
clang-format -i wavelet_tree.cpp

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 wavelet_tree.cpp
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address wavelet_tree.cpp
clang -Wall -Wextra -Og -ggdb3 -lstdc++ -std=c++20 wavelet_tree.cpp

*/

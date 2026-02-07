
#include <algorithm>
#include <cassert>
#include <iostream>
#include <format>
#include <random>
#include <vector>

// Wavelet Tree
// Subrange query series of k-th largest element
// Kudos: bqi343

struct wavelet_tree_t
{
    wavelet_tree_t(const std::vector<int>& input)
    {
        _size = int(input.size());
        _left.resize(_size + 1);
        _right.resize(_size + 1);
        build(input, 1, 0, _size - 1);
    }

    // Find index of k-th largest in subrange [lo..hi] of range [begin..end]
    int index_of_kth(int lo, int hi, int k, int index = 1, int begin = 0, int end = _size - 1)
    {
        if (begin == end)
            return begin;
        const int mid = begin + (end - begin) / 2;
        const int left_size = _left[index][hi] - _left[index][lo];
        if (k <= left_size)
            return index_of_kth(_left[index][lo], _left[index][hi], k, begin, mid);

        return index_of_kth(_right[index][lo], _right[index][hi], k - left_size, mid + 1, end);
    }

  private:
    void build(const std::vector<int>& input, int index, int begin, int end) // [begin..end]
    {
        if (end <= begin)
            return;

        auto& next_left = _left[index];
        auto& next_right = _right[index];
        std::vector<int> child[2];
        const int mid = begin + (end - begin) / 2;
        for (const int v : input) {
            child[mid < v].push_back(v);
            next_left.push_back(child[0].size());
            next_right.push_back(child[1].size());
        }

        build(child[0], 2 * index, begin, mid);
        build(child[1], 2 * index + 1, mid + 1, end);
    }

    static int _size;
    std::vector<std::vector<int>> _left, _right;
};

int main(int, char**)
{
}

/*

Format:
clang-format -i wavelet_tree.cpp

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 wavelet_tree.cpp
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address wavelet_tree.cpp
clang -Wall -Wextra -Og -ggdb3 -lstdc++ -std=c++20 wavelet_tree.cpp

*/


#include <algorithm>
#include <assert.h>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <vector>

// Find maximal element on arbitrary subrange

struct maxelem_segment_tree
{
  public:
    maxelem_segment_tree(const std::vector<int>& input) {
        const int input_size = (int)input.size();
        while (offset < input_size) offset *= 2;
        nodes.assign(2 * offset, MIN32);
        std::copy(input.begin(), input.end(), nodes.begin() + offset);
        for (int n = offset - 1; n > 0; --n)
            nodes[n] = std::max(nodes[2 * n], nodes[2 * n + 1]);
    }

    int query(int lower, int upper) { // range [lower, upper)
        int ret{MIN32};
        for (lower += offset, upper += offset; lower < upper; lower >>= 1, upper >>= 1) { 
            if (lower & 1)
                ret = std::max(ret, nodes[lower++]); 
            if (upper & 1)
                ret = std::max(ret, nodes[--upper]);
        }

        return ret;
    }

    static const int MIN32 = std::numeric_limits<int>::min();
  private:
    std::vector<int> nodes;
    int offset{1};
};

void fuzz()
{
    constexpr int max = 1000;
    constexpr int trials = 1000;
    constexpr int size = 763783;
    const auto rand_in_range = [](const int ubound){std::random_device seed;std::mt19937 gen{seed()};std::uniform_int_distribution<int> dist(0,ubound-1);return dist(gen);};

    std::vector<int> input(size);
    for (int i = 0; i < size; ++i)
        input[i] = rand_in_range(max);

    maxelem_segment_tree mst{input};
    for (int t = 0; t < trials; ++t) {
        int lower = rand_in_range(size);
        int upper = rand_in_range(size);
        if (lower > upper) std::swap(lower, upper);
        int ret{maxelem_segment_tree::MIN32};
        for (int i = lower; i < upper; ++i)
            ret = std::max(ret, input[i]);
        assert(ret == mst.query(lower, upper));
    }
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    maxelem_segment_tree mst = std::vector<int>{1, 15, 3, 7, 11};
    assert(mst.query(0, 5) == 15);
    assert(mst.query(1, 4) == 15);
    assert(mst.query(2, 5) == 11);
    assert(mst.query(2, 5) == 11);
    assert(mst.query(2, 4) == 7);
    assert(mst.query(2, 3) == 3);
    assert(mst.query(0, 1) == 1);

    fuzz();
    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 segment_tree.cpp -o segment_tree.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 segment_tree.cpp -o segment_tree.o

Run:
py.exe interactive_runner.py py.exe segment_tree_testing_tool.py 1 -- segment_tree.exe
segment_tree.exe < segment_tree.in

Input:


Output:


*/

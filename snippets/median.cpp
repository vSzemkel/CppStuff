#include <cassert>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <vector>

static std::mt19937 g_gen{std::random_device{}()};

template <typename T = int>
static auto distribution(const T ubound) {
    std::uniform_int_distribution<T> dist(0, ubound - 1);
    return dist;
}

const auto median_sorted = [](const std::vector<int>& data) -> double {
    const auto sz = data.size();
    const auto half = sz / 2;
    if (sz & 1)
        return data[half];
    else 
        return std::midpoint<double>(data[half - 1], data[half]);
};

const auto median_unsorted = [](std::vector<int>& data) -> double {
    assert(!data.empty());
    const auto sz = data.size();
    const auto half = sz / 2;

    std::nth_element(data.begin(), data.begin() + half, data.end());
    if (sz & 1)
        return data[half];

    return std::midpoint<double>(*std::max_element(data.begin(), data.begin() + half), data[half]);
};

constinit const int N = 100000;

int main(int, char**) {
    std::vector<int> data(N);
    auto dist = distribution<int64_t>(1e09);
    for (auto& d : data)
        d = dist(g_gen);

    // even case
    auto from_nth = median_unsorted(data);
    std::sort(data.begin(), data.end());
    auto from_sorted = median_sorted(data);
    assert(from_nth == from_sorted);

    // odd case
    data.push_back(dist(g_gen));
    from_nth = median_unsorted(data);
    std::sort(data.begin(), data.end());
    from_sorted = median_sorted(data);
    assert(from_nth == from_sorted);
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 median.cpp -o median.exe

*/

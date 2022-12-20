#include <cassert>
#include <iostream>
#include <limits>
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
    else {
        const auto a = data[half - 1];
        return a + double(data[half] - a) / 2;
    }
};

const auto median_unsorted = [](std::vector<int>& data) -> double {
    const auto sz = data.size();
    const auto half = sz / 2;
    if (sz & 1) {
        std::nth_element(data.begin(), data.begin() + half, data.end());
        return data[half];
    } else {
        std::nth_element(data.begin(), data.begin() + half - 1, data.end());
        const auto a = data[half - 1];
        std::nth_element(data.begin(), data.begin() + half, data.end());
        return a + double(data[half] - a) / 2;
    }
};

constinit const int N = 10000;

int main(int, char**) {
    std::vector<int> data(N);
    auto dist = distribution<int64_t>(1e09);
    for (auto& d : data)
        d = dist(g_gen);

    const auto from_nth = median_unsorted(data);
    std::sort(data.begin(), data.end());
    const auto from_sorted = median_sorted(data);

    assert(from_nth == from_sorted);
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 median.cpp -o median.exe

*/

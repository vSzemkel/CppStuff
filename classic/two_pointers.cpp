
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <random>
#include <vector>

// Two pointers technique
// https://www.geeksforgeeks.org/two-pointers-technique/


constexpr const int sum = 50;
constexpr const int size = 30;
constexpr const int range = 60;

const auto rand_in_range = [](const int64_t ubound){std::random_device seed;std::mt19937 gen{seed()};std::uniform_int_distribution<int64_t> dist(0,ubound-1);return dist(gen);};

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

int main(int, char**)
{
    std::vector<int> data;
    std::generate_n(std::back_inserter(data), size, []{ return rand_in_range(range); });
    std::sort(data.begin(), data.end());
    print(data);

    int lb{0}, ub{size - 1};
    while (lb < ub) {
        const auto cur = data[lb] + data[ub];
        if (cur == sum) break;
        if (cur < sum) ++lb;
        else --ub;
    }

    assert(lb == ub || data[lb] + data[ub] == sum);
    std::cout << lb << ' ' << ub << " PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 two_pointers.cpp -o two_pointers.exe

*/

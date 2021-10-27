
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

// Local maximum (Ternary search)
// Given a vector find index of any local maximum value
// see: sinusoid_min in \kickstart\2020\combination_lock.cpp

constexpr const int N = 10000;
std::vector<int> samples;

const auto rand_in_range = [](const int64_t ubound){std::random_device seed;std::mt19937 gen{seed()};std::uniform_int_distribution<int64_t> dist(0,ubound-1);return dist(gen);};

static int local_maximum(const int left, const int right)
{
    if (right - left < 4)
        return std::max_element(samples.begin(), samples.end()) - samples.begin();

    const int slice = (right - left) / 3;
    const int midl = left + slice;
    const int midr = midl + slice;

    return (samples[midl] < samples[midr]) ? local_maximum(midl, right) : local_maximum(left, midr);
}

int main(int, char**)
{
    samples.resize(N);
    std::iota(samples.begin(), samples.end(), 1);
    std::rotate(samples.begin(), samples.begin() + int(rand_in_range(N)), samples.end());

    const int lm = local_maximum(0, N);
    assert(samples[lm] == N);
    std::reverse(samples.begin(), samples.end());
    const int rev_lm = local_maximum(0, N);
    assert(samples[rev_lm] == N);
    assert(lm + rev_lm == N - 1);
    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 local_maximum.cpp -o local_maximum.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address local_maximum.cpp -o local_maximum

*/

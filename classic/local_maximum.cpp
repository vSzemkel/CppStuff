
#include <algorithm>
#include <assert.h>
#include <numeric>
#include <random>
#include <vector>

// Local maximum
// Given a vector find index of any local maximum value


constexpr const int N = 10000;
std::vector<int> samples;

const auto rand_in_range = [](const int64_t ubound){std::random_device seed;std::mt19937 gen{seed()};std::uniform_int_distribution<int64_t> dist(0,ubound-1);return dist(gen);};

static int local_maximum(const int left, const int right)
{
    if (right - left < 2)
        return samples[left] < samples[right] ? right : left;

    const int mid = (left + right) / 2;
    if (samples[left] <= samples[mid] && samples[mid] <= samples[right])
        return local_maximum(mid, right);
    if (samples[left] >= samples[mid] && samples[mid] >= samples[right])
        return local_maximum(left, mid);

    const int lcan = local_maximum(left, mid);
    const int rcan = local_maximum(mid, right);
    if (lcan == rcan)
        return mid;
    return lcan != mid ? lcan : rcan;
}

int main(int, char**)
{
    samples.resize(N + 1);
    std::iota(samples.begin(), samples.end(), 0);
    std::rotate(samples.begin(), samples.begin() + int(rand_in_range(N)), samples.end());
    const int lm = local_maximum(0, N);
    assert(samples[lm] == N || lm == N);
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 local_maximum.cpp -o local_maximum.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address local_maximum.cpp -o local_maximum

*/

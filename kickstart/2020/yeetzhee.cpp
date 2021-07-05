
#include <algorithm>
#include <array>
#include <assert.h>
#include <bitset>
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
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <stdlib.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <vector>

// Yeetzhee
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ff48/00000000003f4dea

// Expected value of throwing one of G values with a dice with M possible values is
// X = 1 + (M - G) / M * X
// X * M = M + (M - G) * X
// X * G = M; X == M / G

int N, M, K;
std::vector<int> need;
std::map<std::vector<int>, double> cache;

static double compute(std::vector<int>& got) { // inspired by vlvlovch
    if (got == need)
        return 0.;
    const auto cached = cache.find(got);
    if (cached != cache.end())
        return cached->second;

    int full{0}; // count of fully taken groups
    for (int i = 0; i < M; ++i) {
        if (got[i] == need[i])
            ++full;
        if (got[i] < need[i])
            while (i < M - 1 && got[i] == got[i + 1]) // maybe swap equal groups to reduce full
                ++i;
    }

    double ret = double(M) / (M - full); // repeat rolling until not full group hit
    for (int i = 0; i < M; ++i) {
        if (got[i] == need[i])
            continue;
        assert(got[i] < need[i]);
        const int j{i};
        while (i < M - 1 && got[i] == got[i + 1]) // maybe swap equal groups to reduce full
            ++i;

        ++got[j];
        ret += double(i - j + 1) / (M - full) * compute(got); // one of got[i] sized groups out of all not full
        --got[j];
    }

    return cache[got] = ret;
}

static void solve() {
    std::cin >> N >> M >> K;
    need.resize(K);
    for (auto& k : need)
        std::cin >> k;
    std::sort(need.begin(), need.end(), [](const int i, const int j){
        return i > j;
    });
    need.resize(M); // zeros for indexes >= K

    cache.clear();
    std::vector<int> got(M); // counts of throwed values
    std::cout << compute(got);
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": " << std::fixed << std::setprecision(9); solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 yeetzhee.cpp -o yeetzhee.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address yeetzhee.cpp -o yeetzhee

Run:
yeetzhee.exe < yeetzhee.in

Input:

2
3 6 2
1
2
5 2 1
5

Output:

Case #1: 4.700000000
Case #2: 9.000000000

*/

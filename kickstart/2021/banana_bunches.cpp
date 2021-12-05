
#include <algorithm>
#include <array>
#include <assert.h>
#include <iostream>
#include <map>
#include <numeric>
#include <unordered_map>
#include <utility>
#include <vector>

// Banana Bunches
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000004362d6/00000000008b44ef

constexpr const int INF = 1e09;

std::vector<int64_t> trees, partial;
const auto range_sum = [](const int i, const int j){return partial[j + 1] - partial[i];};

// Consider two ranges [0..j][j+1..N-1]
// INVARIANT: For each sum we know minimal lenght in right range
// For each range [i..j] we can update best score in constant time
// For each range [j..N] we update the invariant
static void solve() {
    int64_t N, K;
    std::cin >> N >> K;
    trees.resize(N);
    for (auto& t : trees)
        std::cin >> t;

    int ret{INF};
    std::vector<int> best(K + 1, INF);
    best[0] = 0;
    for (int j = N - 1; j >= 0; --j) {
        int64_t prefSum{0}, sufSum{0};
        for (int i = j; i >= 0; --i) {
            prefSum += trees[i];
            if (prefSum > K)
                break;
            ret = std::min(ret, j - i + 1 + best[K - prefSum]);
        }
        for (int k = j; k < N; ++k) {
            sufSum += trees[k];
            if (sufSum > K)
                break;
            best[sufSum] = std::min(best[sufSum], k - j + 1);
        }
    }

    if (ret == INF)
        ret = -1;

    std::cout << ret;
}

static void solve_set2() { // TLE on Set3
    int64_t N, K;
    std::cin >> N >> K;
    trees.resize(N);
    partial.resize(N + 1);
    for (auto& t : trees)
        std::cin >> t;
    std::partial_sum(trees.begin(), trees.end(), partial.begin() + 1);

    if (range_sum(0, N - 1) < K) {
        std::cout << -1;
        return;
    }

    int ret = INF;
    std::unordered_map<int64_t, std::map<int, std::array<int, 2>>> sections; // sum->len->{leftmost_start, rightmost_start}
    for (int i = 0; i < N; ++i)
        for (int j = i; j < N; ++j) {
            const auto len = j - i + 1;
            const auto rs = range_sum(i, j);
            if (rs >= K) {
                if (rs == K)
                    ret = std::min(ret, len);
                break;
            }

            auto& rsmap = sections[rs];
            if (rsmap.find(len) == rsmap.end())
                rsmap[len] = {i, i};
            else {
                auto& item = rsmap[len];
                if (i < item[0])
                    item[0] = i;
                else if (item[1] < i)
                    item[1] = i;
            }
        }

    for (int l = 1; l <= K / 2; ++l) {
        const auto& lsum = sections[l];
        const auto& rsum = sections[K - l];

        for (auto lit = lsum.begin(), rit = rsum.begin(); lit != lsum.end() && rit != rsum.end() && lit->first + rit->first < K;) {
            if (lit->second[0] + lit->first < rit->second[1])
                ret = std::min(ret, lit->first + rit->first);
            if (rit->second[0] + rit->first < lit->second[1])
                ret = std::min(ret, lit->first + rit->first);
            if (ret < INF)
                break;

            const int newlen = lit->first + std::next(rit)->first;
            if (newlen < rit->first + std::next(lit)->first)
                rit = std::next(rit);
            else
                lit = std::next(lit);
        }
    }

    if (ret == INF)
        ret = -1;

    std::cout << ret;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 banana_bunches.cpp -o banana_bunches.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address banana_bunches.cpp -o banana_bunches

Run:
banana_bunches.exe < banana_bunches.in

Input:

4
6 8
1 2 3 1 2 3
4 10
6 7 5 2
6 8
3 1 2 1 3 1
4 6
3 1 2 0

Output:

Case #1: 3
Case #2: -1
Case #3: 4
Case #4: 3

*/

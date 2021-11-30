
#include <array>
#include <cassert>
#include <iostream>
#include <numeric>
#include <unordered_set>
#include <vector>

// Subset sum
// Given a set of integers determine if particular sum can be obtained by subset sum

constexpr const int N = 12;
std::array<int, N> items = {35, 12, 9, 20, 14, 26, 7, 10, 4, 18, 12, 14};
std::vector<bool> dp; // dp[i] == bool(i is a sum of subset of first processed elements)
std::vector<std::vector<bool>> choosen; // choosen[i][j] == bool(i-th element is included in subset summing to j)

static auto all_sums() {
    std::unordered_set<int> ret[2];
    ret[0].insert(0);

    for (int i = 0; i < N; ++i) {
        const int z = i & 1;
        const auto& src = ret[z];
        auto& dst = ret[z ^ 1];
        dst = src;
        for (const int s : src)
            dst.insert(s + items[i]);
    }

    return ret[N & 1];
}

static bool subset_sum(const int target) {
    choosen.assign(N + 1, std::vector<bool>(target + 1));
    dp.assign(target + 1, false);
    dp[0] = true;
    for (int i = 0; i < N; ++i)
        for (int t = target; t >= items[i]; --t) {
            choosen[i + 1][t] = !dp[t] && dp[t - items[i]];
            dp[t] = dp[t] | dp[t - items[i]];
        }

    return dp[target];
}

int main(int, char**)
{
    const auto as = all_sums();
    for (const auto s : as)
        assert(subset_sum(s));

    const int target = std::accumulate(items.begin(), items.begin() + items.size() / 2, 0);
    //int target{0}; for (int i = 0; i < N; i += 2) target += items[i];
    assert(subset_sum(target));
    bool first{true};
    int t{target};
    for (int z = N; z; --z)
        if (choosen[z][t]) {
            const int val = items[z - 1];
            if (first) {
                std::cout << val;
                first = false;
            } else
                std::cout << " + " << val;
            t -= val;
        }
    std::cout << " == " << target << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 subset_sum.cpp -o subset_sum.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address subset_sum.cpp -o subset_sum

*/

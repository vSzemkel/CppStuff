
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <vector>

// Oversized Pancake Choppers
// https://codingcompetitions.withgoogle.com/codejam/round/000000000019fef4/00000000003172d1

using slice_t = std::pair<int64_t, int64_t>; // reduced fraction: <numerator, denominator>

int g_N, g_D;
std::vector<int64_t> g_sectors;

const auto can_slice_target = [](const slice_t& t) noexcept -> bool {
    int64_t to_cut{g_D};
    for (int i = g_sectors.size() - 1; i >= 0; --i) {
        const int64_t portions = g_sectors[i] * t.second / t.first;
        if (portions == 0) break;
        to_cut -= portions;
        if (to_cut <= 0) return true;
    }

    return to_cut <= 0;
};

int solve() {
    std::map<slice_t, int> slice_index;
    std::vector<std::set<int>> fully_usable_sectors; // slice index -> set of sector indexes
    std::sort(g_sectors.begin(), g_sectors.end());

    int si{0};
    const auto comp_slice = [](const slice_t& s1, const slice_t& s2){ return s1.first * s2.second < s2.first * s1.second; };
    std::set<slice_t, decltype(comp_slice)> target_sizes(std::move(comp_slice));
    for (int64_t i = 1; i <= g_D; ++i)
        for (int s = 0; s < g_N; ++s) {
            const auto gd = std::gcd(g_sectors[s], i);
            const auto empres = target_sizes.emplace(g_sectors[s] / gd, i / gd);
            if (empres.second) {
                slice_index[*empres.first] = si++;
                fully_usable_sectors.push_back({});
            }
            fully_usable_sectors[slice_index[*empres.first]].insert(s);
        }

    int ret{g_D - 1};
    const auto target_2big = std::lower_bound(target_sizes.begin(), target_sizes.end(), true, [](const auto t, bool ok){ return can_slice_target(t) == ok; });
    for (auto tit = target_sizes.begin(); tit != target_2big; ++tit) {
        int ca{g_D};
        int64_t served{0};
        const auto chunk = *tit;
        for (const auto si : fully_usable_sectors[slice_index[chunk]]) {
            const auto slice = g_sectors[si];
            if (served * chunk.second < g_D * chunk.first) {
                served += slice;
                if (served * chunk.second <= g_D * chunk.first) --ca;
            }
        }

        ret = std::min(ret, ca);
    }

    return ret;
}

int solve_slow() {
    std::sort(g_sectors.begin(), g_sectors.end());

    // compute max target size
    const auto comp_slice = [](const slice_t& s1, const slice_t& s2){ return s1.first * s2.second < s2.first * s1.second; };
    std::set<slice_t, decltype(comp_slice)> target_sizes(std::move(comp_slice));
    for (int64_t i = 1; i <= g_D; ++i)
        for (const auto s : g_sectors) {
            const auto gd = std::gcd(s, i);
            target_sizes.emplace(s / gd, i / gd);
        }

    int ret{g_D - 1};
    const auto target_2big = std::lower_bound(target_sizes.begin(), target_sizes.end(), true, [](const auto t, bool ok){ return can_slice_target(t) == ok; });
    for (auto tit = target_sizes.begin(); tit != target_2big; ++tit) {
        int ca{g_D};
        int64_t served{0};
        const auto chunk = *tit;
        std::vector<int64_t> copy(g_N);
        const auto size = std::copy_if(g_sectors.begin(), g_sectors.end(), copy.begin(), [chunk](const auto slice){ return ((slice * chunk.second) % chunk.first) == 0; }) - copy.begin();
        // total == g_D * chunk.first / chunk.second
        for (int j = 0; j < size && served * chunk.second < g_D * chunk.first; ++j) {
            served += copy[j];
            if (served * chunk.second <= g_D * chunk.first) --ca;
        }

        ret = std::min(ret, ca);
    }

    return ret;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cin >> g_N >> g_D;
        g_sectors.resize(g_N);
        for (int i = 0; i < g_N; ++i)
            std::cin >> g_sectors[i];
        std::cout << "Case #" << g << ": " << solve() << "\n";
        //std::cout << "Case #" << g << ": " << solve_slow() << "\n";
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 oversized_pancake_choppers.cpp -o oversized_pancake_choppers.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 oversized_pancake_choppers.cpp -o oversized_pancake_choppers.o

Run:
oversized_pancake_choppers.exe < oversized_pancake_choppers.in

Input:

10
5 3
1 1 1 1 1
6 5
2 2 2 2 3 3
3 5
6 9 9
1 10
100
7 6
2 2 2 4 4 4 7
10 7
2 2 11 13 29 44 91 22 13 53
1 3
1
5 2
10 5 359999999999 123456789 10
2 3
8 4
3 2
1 2 3

Output:

Case #1: 0
Case #2: 1
Case #3: 3
Case #4: 9
Case #5: 2
Case #6: 4
Case #7: 2
Case #8: 0
Case #9: 1
Case #10: 1

*/

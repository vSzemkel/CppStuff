
#include <algorithm>
#include <array>
#include <iostream>
#include <set>
#include <vector>

// Contention
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050e01/0000000000069881#problem

template <typename T, typename U>
T last_true(T lo, T hi, U f) {
    lo--;
    while (lo < hi) { // find last index such that f is true
        const T mid = lo + (hi - lo + 1) / 2; // this will work for negative numbers too
        f(mid) ? lo = mid : hi = mid - 1;
    }
    return lo;
}

/**
 * @brief This is variant of sweep line with greedy expanding in-the-middle reservations
 */
static void solve() { // by wifi
    int N, Q;
    std::cin >> N >> Q;
    int max{N};
    std::vector<std::array<int, 2>> ranges(Q);
    for (auto& [l, r] : ranges) {
        std::cin >> l >> r; --l;
        max = std::min(max, r - l);
    }
    std::sort(ranges.begin(), ranges.end());

    const auto check = [&](const int k) -> bool {
        int ord{0}, last{0};
        std::vector<int> score(Q);
        std::set<std::array<int, 2>> inmid;
        for (const auto& [l, r] : ranges) {
            while (!inmid.empty()) {
                const auto [mr, i] = *inmid.begin();
                if (mr <= l) {
                    score[i] += std::max(mr - last, 0);
                    last = std::max(last, mr);
                    inmid.erase(inmid.begin());
                } else
                    break;
            }
            if (last < l) {
                if (!inmid.empty())
                    score[(*inmid.begin())[1]] += l - last;
                last = l;
            }

            auto [it, _] = inmid.insert({r, ord++});
            if (it == inmid.begin())
                continue;
            --it;
            while (score[(*it)[1]] >= k) {
                if (it == inmid.begin()) {
                    inmid.erase(it);
                    break;
                } else
                    it = std::prev(inmid.erase(it));
            }
        }

        for (const auto& [r, i] : inmid) {
            score[i] += std::max(r - last, 0);
            last = std::max(last, r);
        }

        return std::all_of(score.begin(), score.end(), [k](const int s){ return k <= s; });
    };

    std::cout << last_true(0, max, check);
}

/**
 * @brief Last reservation can be greedy maximized
 */
static void solve_set1() {
    int N, Q;
    std::cin >> N >> Q;
    std::vector<std::array<int, 2>> ranges(Q);
    for (auto& r : ranges)
        std::cin >> r[0] >> r[1];

    std::vector<bool> used(Q);
    std::vector<int> covered(N);
    for (const auto& r : ranges)
        for (int i = r[0] - 1; i < r[1]; ++i)
            ++covered[i];

    int ret{N};
    for (int z = Q; z; --z) {
        int max{0}, pos{-1};
        for (int i = 0; i < Q; ++i) {
            if (used[i]) continue;
            const int taken = std::count_if(covered.begin() + ranges[i][0] - 1, covered.begin() + ranges[i][1], [](const int c){ return c == 1; });
            if (max < taken) {
                max = taken;
                pos = i;
            }
        }

        ret = std::min(ret, max);

        if (~pos) {
            used[pos] = true;
            for (int i = ranges[pos][0] - 1; i < ranges[pos][1]; ++i)
                --covered[i];
        } else
            break;
    }

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
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 contention.cpp -o contention.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address contention.cpp -o contention

Run:
contention.exe < contention.in

Input:

3
5 3
1 2
3 4
2 5
30 3
10 11
10 10
11 11
10 4
1 8
4 5
3 6
2 7

Output:


*/

/*
ID: marcin.8
LANG: C++17
TASK: theme
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=DMzbY6AZYHC&S=theme
*/

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdlib>
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
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <vector>

std::ifstream task_in("theme.in");
std::ofstream task_out("theme.out");

template <typename T, typename U>
T last_true(T lo, T hi, U f) {
    lo--;
    assert(lo <= hi); // assuming f is decreasing
    while (lo < hi) { // find last index such that f is true
        const T mid = lo + (hi - lo + 1) / 2; // this will work for negative numbers too
        f(mid) ? lo = mid : hi = mid - 1;
    }
    return lo;
}

/**
 * @brief Searches for prefixes of needle in the haystack string
 * 
 * @param s string modelled as "needle$haystack"
 * @return std::vector<int> ret[n] == k iff k-prefix of needle starts at haystack[n - 2 * size(needle)]
 */
static std::vector<int> kmp(const std::vector<int>& s) {
    const int n = int(s.size());
    std::vector<int> ret(n);
    for (int k = 0, i = 1; i < n; ++i) { // k is lenght of already found proper (non identity) bound 
        while (k > 0 && s[k] != s[i])
            k = ret[k - 1]; // books say there should be ret[k-1] - counterexample needed
        if (s[k] == s[i])
            ++k;
        ret[i] = k;
    }

    return ret;
}

int main(int, char**)
{
    int N, p;
    task_in >> N >> p; --N;
    std::vector<int> diffseq(N);
    for (auto& x : diffseq) {
        int n; task_in >> n;
        x = n - p;
        p = n;
    }

    const auto check = [&](const int k) -> bool {
        const auto ub = N - 2 * k;
        std::vector<int> nh(N + 1);
        for (int i = 0; i <= ub; ++i) {
            nh.clear();
            nh.insert(nh.end(), diffseq.begin() + i, diffseq.begin() + i + k);
            nh.push_back(1e09);
            nh.insert(nh.end(), diffseq.begin() + i + k, diffseq.end());
            const auto findings = kmp(nh);
            if (std::find(findings.begin(), findings.end(), k) != findings.end())
                return true;
        }

        return false;
    };

    auto ans = last_true(0, N / 2, check);
    if (2 * ans < N)
        ++ans;
    if (ans < 5)
        ans = 0;

    task_out << ans << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 theme.cpp -o theme.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address theme.cpp -o theme

Input:

30
25 27 30 34 39 45 52 60 69 79 69 60 52 45 39 34 30 26 22 18 82 78 74 70 66 67 64 60 65 80

Output:


*/

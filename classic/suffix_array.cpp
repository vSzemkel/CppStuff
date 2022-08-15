
#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

// Suffix Array
// https://cp-algorithms.com/string/suffix-array.html

struct suffix_array_t {
    auto sort_cyclic_shifts(const std::string& s) { // O(NlogN)
        const int alphabet = 256;
        const int n = int(s.size());
        // preprocess single characters with counting sort
        std::vector<int> rank(n), cls(n), counts(std::max(alphabet, n));
        for (int i = 0; i < n; ++i)
            ++counts[s[i]];
        std::partial_sum(counts.cbegin(), counts.cend(), counts.begin());
        for (int i = 0; i < n; ++i)
            rank[--counts[s[i]]] = i;

        int classes{1};
        cls[rank[0]] = 0;
        for (int i = 1; i < n; ++i) {
            if (s[rank[i]] != s[rank[i - 1]])
                ++classes;
            cls[rank[i]] = classes - 1;
        }

        std::vector<int> rn(n), cn(n);
        for (int h = 0; (1 << h) < n; ++h) {
            for (int i = 0; i < n; ++i) {
                rn[i] = rank[i] - (1 << h);
                if (rn[i] < 0)
                    rn[i] += n;
            }

            std::fill(counts.begin(), counts.begin() + classes, 0);
            for (int i = 0; i < n; ++i)
                ++counts[cls[rn[i]]];
            std::partial_sum(counts.cbegin(), counts.cbegin() + classes, counts.begin());
            for (int i = n - 1; ~i; --i)
                rank[--counts[cls[rn[i]]]] = rn[i];

            classes = 1;
            cn[rank[0]] = 0;
            for (int i = 1; i < n; ++i) {
                const std::pair<int, int> prev = {cls[rank[i - 1]], cls[(rank[i - 1] + (1 << h)) % n]};
                const std::pair<int, int> cur = {cls[rank[i]], cls[(rank[i] + (1 << h)) % n]};
                if (cur != prev)
                    ++classes;
                cn[rank[i]] = classes - 1;
            }

            cls.swap(cn);
        }

        return rank;
    }

    std::vector<int> suffix_array(const std::string& s) {
        const auto sorted_shifts = sort_cyclic_shifts(s + '$');
        return {sorted_shifts.begin() + 1, sorted_shifts.end()};
    }
};

auto naive_suffix_array(const std::string& s) { // O(N2logN)
    const auto size = s.size();
    std::vector<std::pair<std::string_view, int>> suffixes(size);
    for (size_t i = 0; i < size; ++i)
        suffixes[i] = {std::string_view{s.data() + i, size - i}, i};
    std::sort(suffixes.begin(), suffixes.end());

    std::vector<int> ret(size);
    std::transform(suffixes.cbegin(), suffixes.cend(), ret.begin(), [](const auto& p){ return p.second; });
    return ret;
}

int main(int, char**)
{
    const std::string s = "ababbaaaabbaababaaaabaaabbbab";
    const auto order = naive_suffix_array(s);
    suffix_array_t sa;
    const auto order2 = sa.suffix_array(s);
    assert(order == order2);
    for (const int o : order)
        std::cout << s.substr(o) << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 suffix_array.cpp -o suffix_array.exe

*/

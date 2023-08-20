
#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>

// Minimum Window Substring
// https://leetcode.com/problems/minimum-window-substring

static std::string_view solve(std::string_view S, std::string_view T) {
    std::array<int, 123> tmap{}, cmap{};
    const auto check = [&tmap](const std::array<int, 123>& m) {
        const int start = 'A';
        const int count = 'z' - 'A' + 1;
        auto t = tmap.data() + start;
        auto g = m.data() + start;
        for (int z = count; z; --z)
            if (*g++ < *t++)
                return false;
        return true;
    };

    const auto szs = int(S.size()), latest_start = szs - int(T.size());
    int last = -1, start = -1, best = 1e09;
    for (const auto& c : T)
        ++tmap[c];
    for (int i = 0; i <= latest_start; ++i) {
        while (i <= latest_start && tmap[S[i]] == 0)
            ++i;

        if (check(cmap)) {
            const int cur = last - i + 1;
            if (cur < best) {
                best = cur;
                start = i;
            }
        }

        const auto range = std::min(szs, i + best - 1);
        while (last < range) {
            ++cmap[S[++last]];
            if (check(cmap)) {
                best = last - i + 1;
                start = i;
                break;
            }
        }

        --cmap[S[i]];
    }

    return ~start ? std::string_view(S.data() + start, best) : std::string_view{};
}

static std::string_view solve_naive(std::string_view S, std::string_view T, std::unordered_map<char, int>& smap, std::unordered_map<char, int>& tmap) {
    if (std::any_of(tmap.begin(), tmap.end(), [&](const auto& kv){ return kv.second > smap[kv.first]; }))
        return {};

    int left{};
    while (tmap[S[left]] == 0) ++left;
    int right{int(S.size()) - 1};
    while (tmap[S[right]] == 0) --right;
    S = S.substr(left, right - left + 1);

    const auto l = S.front();
    const auto r = S.back();
    if (smap[l] <= tmap[l] && smap[r] <= tmap[r])
        return S;

    const auto len = S.size() - 1;
    --smap[l];
    const auto lans = solve_naive(S.substr(1, len), T, smap, tmap);
    ++smap[l];
    --smap[r];
    const auto rans = solve_naive(S.substr(0, len), T, smap, tmap);
    ++smap[r];
    if (lans.empty())
        return rans;
    if (rans.empty())
        return lans;
    if (lans.size() < rans.size())
        return lans;
    return rans;
}

static void io_handler() {
    std::string S, T;
    std::cin >> S >> T;
    assert(!T.empty());

    // std::unordered_map<char, int> smap, tmap;
    // for (const auto& c : S) ++smap[c];
    // for (const auto& c : T) ++tmap[c];
    // std::cout << solve_naive(S, T, smap, tmap);

    const auto ans = solve(S, T);
    std::cout << ans << ' ' << ans.size();
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; io_handler(); std::cout << '\n';
    }
}

/*

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 minimum_window_substring.cpp -o minimum_window_substring.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address minimum_window_substring.cpp -o minimum_window_substring

Run:
minimum_window_substring.exe < minimum_window_substring.in

Input:

3
ADOBECODEBANC ABC
cabccbbcbabaccccbcccbbbbacccccbbbabababc bcabaabbaaaca
cabbabaaaaccbbaabccccbabcacbbcbaacacbacbbbacabac aacbcbcbbcbccabaabcbccabb

Output:

Case #1: BANC
Case #2: abccbbcbabaccccbcccbbbbacccccbbbababa
Case #3: bccccbabcacbbcbaacacbacbbb

*/

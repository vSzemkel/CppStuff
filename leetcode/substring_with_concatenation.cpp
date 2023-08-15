
#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
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

// Substring with Concatenation of All Words
// https://leetcode.com/problems/substring-with-concatenation-of-all-words/?envType=study-plan-v2&envId=top-interview-150

static std::vector<int> solve(const std::string& text, const std::vector<std::string>& words) {
    const int N = int(words.size());
    const int D = int(words.front().size());
    std::vector<int> scan(N, -1); // scan[i] == j iff text[i] is start of words[j] or -1 if not for any word
    int word_index{-1};
    for (const auto& w : words) {
        ++word_index;
        int search_from{};
        auto found = text.find(w, search_from);
        while (found != std::string::npos) {
            scan[found] = word_index;
            search_from = found + 1;
            found = text.find(w, search_from);
        }
    }

    std::vector<int> ret;
    const auto pattern_size = N * D;
    for (int i = 0; i < D; ++i) {
        std::vector<bool> window(N);
    }

    return ret;
}

static void io_handler() {
    int N;
    std::string S;
    std::cin >> N >> S;
    assert(N > 0);
    std::vector<std::string> words(N);
    for (auto& w : words)
        std::cin >> w;
    const auto size = words.front().size();
    assert(std::all_of(words.begin(), words.end(), [&](const auto& w){ return w.size() == size; }));

    const auto ans = solve(S, words);
    for (const auto& p : ans)
        std::cout << p << ' ';
    std::cout << '\n';
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
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 substring_with_concatenation.cpp -o substring_with_concatenation.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address substring_with_concatenation.cpp -o substring_with_concatenation

Run:
substring_with_concatenation.exe < substring_with_concatenation.in

Input:


Output:


*/

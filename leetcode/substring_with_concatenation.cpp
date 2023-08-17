
#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

// Substring with Concatenation of All Words
// https://leetcode.com/problems/substring-with-concatenation-of-all-words/?envType=study-plan-v2&envId=top-interview-150

static std::vector<int> solve(const std::string& text, const std::vector<std::string>& words) {
    const auto L = int(text.size());
    const auto N = int(words.size());
    const auto D = int(words.front().size());
    if (L < N * D)
        return {};

    std::unordered_map<int, int> expected, window; // {index -> count}
    std::unordered_map<std::string_view, int> dictionary; // {string -> index}
    int word_index;
    for (const auto& w : words) {
        if (dictionary.contains(w))
            word_index = dictionary[w];
        else {
            word_index = int(dictionary.size());
            dictionary[w] = word_index;
        }
        ++expected[word_index];
    }

    const auto scan = [&](const int pos){
        if (pos + D <= L) {
            std::string_view s(text.data() + pos, D);
            if (dictionary.contains(s))
                return dictionary[s];
        }

        return -1;
    };

    std::vector<int> ret;
    for (int i = 0; i < D; ++i) {
        window.clear();
        for (int j = 0; j < N; ++j) {
            const auto pos = i + D * j;
            if (const auto p = scan(pos); p != -1)
                ++window[p];
        }

        int first{i}, last{i + (N - 1) * D};
        while (true) {
            if (window == expected)
                ret.push_back(first);
            if (last + D >= L)
                break;
            if (const auto p = scan(first); p != -1)
                --window[p];
            first += D;
            last += D;
            if (const auto p = scan(last); p != -1)
                ++window[p];
        }
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

2
3 barfoofoobarthefoobarman
bar foo the
4 wordgoodgoodgoodbestword
word good best good

Output:

Case #1: 6 9 12 
Case #2: 8

*/

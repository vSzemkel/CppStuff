
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>

// Distinct Subsequences
// https://leetcode.com/problems/distinct-subsequences/

int patternLenth, textLength;
std::string text, pattern;
std::unordered_map<int, int> trace; // hash -> paths count
std::unordered_map<char, std::set<int>> memo;

int numDistinctInternal(const int textIndex, const int patternIndex)
{
    if (textLength - textIndex < patternLenth - patternIndex)
        return 0;

    const int hash = (textIndex << 10) + patternIndex;
    if (patternIndex == patternLenth)
        return trace[hash] = 1;

    const auto it = trace.find(hash);
    if (it != trace.end())
        return it->second;

    int ret{};
    const auto& occurences = memo[pattern[patternIndex]];
    for (auto it = occurences.lower_bound(textIndex); it != occurences.end(); ++it)
        ret += numDistinctInternal(*it + 1, patternIndex + 1);

    return trace[hash] = ret;
}

int numDistinct(std::string s, std::string t)
{
    text = std::move(s);
    pattern = std::move(t);

    for (int i = -1; const char c : text)
        memo[c].insert(++i);

    textLength = int(text.size());
    patternLenth = int(pattern.size());
    return numDistinctInternal(0, 0);
}

static void io_handler()
{
    std::string text, pattern;
    std::cin >> text >> pattern;

    memo.clear();
    trace.clear();
    std::cout << numDistinct(text, pattern);
}

int main(int, char**)
{
    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": ";
        io_handler();
        std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 distinct_subsequences.cpp -o distinct_subsequences.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address distinct_subsequences.cpp -o distinct_subsequences

Run:
distinct_subsequences < distinct_subsequences.in

Input

3
rabbbit rabbit
babgbag bag
adbdadeecadeadeccaeaabdabdbcdabddddabcaaadbabaaedeeddeaeebcdeabcaaaeeaeeabcddcebddebeebedaecccbdcbcedbdaeaedcdebeecdaaedaacadbdccabddaddacdddc bcddceeeebecbc

Output

Case #1: 3
Case #2: 5
Case #3: 700531452

*/

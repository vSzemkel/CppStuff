
#include <iostream>
#include <string_view>
#include <unordered_set>

// find all the words such that they may be composed of a concatenation of others


// case definition - words collection
const std::unordered_set<std::string_view> g_words = {"lecture", "bbb", "palindrom", "aa", "aabbbcccc", "buchwald", "wall", "blum", "buch", "palisade", "wald", "ostrich", "cccc"};

// globals
std::unordered_set<std::string_view> g_cache;

bool can_be_build(const std::string_view& goal)
{
    if (g_cache.count(goal))
        return true;

    for (int len = 1; len < goal.size() - 1; ++len) {
        const std::string_view& prefix = goal.substr(0, len);
        const std::string_view& sufix = goal.substr(len);
        if (g_words.count(prefix) > 0 && (g_words.count(sufix) > 0 || can_be_build(sufix))) {
            g_cache.insert(goal);
            return true;
        }
    }

    return false;
}

int main(int argc, char* argv[])
{
    for (const auto& sv : g_words)
        if (can_be_build(sv))
            std::cout << sv << "\n";

    return 0;
}

/* clang++.exe -Wall -g -std=c++17 find_concatenated.cpp -o find_concatenated.exe

Output:

aabbbcccc
buchwald

*/
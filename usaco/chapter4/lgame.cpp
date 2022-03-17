/*
ID: marcin.8
LANG: C++
TASK: lgame
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=NbC8CVUxWYb&S=lgame
*/

#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iterator>
#include <numeric>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

std::ifstream task_in("lgame.in");
std::ofstream task_out("lgame.out");

std::unordered_map<char, int> charval = {
    {'a', 2}, {'b', 5}, {'c', 4}, {'d', 4}, {'e', 1}, {'f', 6},
    {'g', 5}, {'h', 5}, {'i', 1}, {'j', 7}, {'k', 6}, {'l', 3},
    {'m', 5}, {'n', 2}, {'o', 3}, {'p', 5}, {'q', 7}, {'r', 2},
    {'s', 1}, {'t', 2}, {'u', 4}, {'v', 6}, {'w', 6}, {'x', 7},
    {'y', 5}, {'z', 7}
};

int main(int, char**)
{
    int total{0};
    std::string input;
    task_in >> input;
    for (const char c : input)
        total += charval[c];

    std::string word;
    std::vector<std::string> dict;
    std::ifstream src{"lgame.dict"};
    while (std::getline(src, word))
        dict.push_back(word);

    std::string weighted_input = input;
    std::sort(weighted_input.begin(), weighted_input.end(), [](const char c1, const char c2){
        return charval[c1] > charval[c2];
    });

    int best{0}, size = (1 << int(input.size()));
    std::vector<std::pair<std::string, int>> found; // {string, space position}
    for (int mask = 0; mask < size; ++mask) {
        // remove masked out letters starting from least valuable
        int bit{1}, ans{total};
        std::string letters;
        std::vector<std::pair<std::string, int>> round_findings;
        for (const char c : weighted_input) {
            if ((mask & bit) == 0)
                letters.push_back(c);
            else
                ans -= charval[c];
            bit <<= 1;
        }
        if (ans < best || letters.size() < 3)
            continue;
        // search for single word - check all permutation of letters
        std::sort(letters.begin(), letters.end());
        do {
            if (std::binary_search(dict.begin(), dict.end(), letters))
                round_findings.emplace_back(letters, 0);
        } while (std::next_permutation(letters.begin(), letters.end()));
        // search for pair of words
        const int sz = int(letters.size());
        if (sz > 5) 
            for (int i = 0; i < sz; ++i)
                for (int j = i + 1; j < sz; ++j)
                    for (int k = j + 1; k < sz; ++k) {
                        std::string first, second;
                        std::vector<std::string> fres, sres;
                        for (int d = 0; d < sz; ++d)
                            if (d == i || d == j || d == k)
                                first.push_back(letters[d]);
                            else
                                second.push_back(letters[d]);
                        std::sort(first.begin(), first.end());
                        do {
                            if (std::binary_search(dict.begin(), dict.end(), first))
                                fres.push_back(first);
                        } while (std::next_permutation(first.begin(), first.end()));
                        if (fres.empty())
                            continue;
                        std::sort(second.begin(), second.end());
                        do {
                            if (std::binary_search(dict.begin(), dict.end(), second))
                                sres.push_back(second);
                        } while (std::next_permutation(second.begin(), second.end()));
                        if (sres.empty())
                            continue;
                        std::sort(fres.begin(), fres.end());
                        std::sort(sres.begin(), sres.end());
                        fres.erase(std::unique(fres.begin(), fres.end()), fres.end());
                        sres.erase(std::unique(sres.begin(), sres.end()), sres.end());
                        for (const auto& f : fres)
                            for (const auto& s : sres)
                                if (f < s)
                                    round_findings.emplace_back(f + s, f.size());
                                else
                                    round_findings.emplace_back(s + f, s.size());
                    }

        if (!round_findings.empty()) {
            if (best < ans) {
                best = ans;
                found = std::move(round_findings);
            } else {
                found.reserve(found.size() + round_findings.size());
                std::move(round_findings.begin(), round_findings.end(), std::back_inserter(found));
            }
        }
    }

    if (found.empty())
        task_out << "0\n";
    else {
        std::sort(found.begin(), found.end());
        found.erase(std::unique(found.begin(), found.end()), found.end());
        task_out << best << '\n';
        for (auto& f : found) {
            if (f.second > 0)
                f.first.insert(f.first.begin() + f.second, ' ');
            task_out << f.first << '\n';
        }
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 lgame.cpp -o lgame.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address lgame.cpp -o lgame

Input:

pofax

Output:

16
fox

*/

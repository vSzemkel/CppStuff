
#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

// Knuth Morris Pratt search algorithm

/**
 * @brief Searches for prefixes of needle in the haystack string
 * 
 * @param s string modelled as "needle$haystack"
 * @return std::vector<int> ret[n] == k iff k-prefix of needle starts at haystack[n - 2 * size(needle)]
 */
static std::vector<int> kmp(const std::string& s) {
    const int n = int(s.size());
    std::vector<int> ret(n);
    for (int k = 0, i = 1; i < n; ++i) { // k is lenght of already found proper (non identity) bound 
        while (k > 0 && s[k] != s[i])
            k = ret[k]; // books say there should be ret[k-1] - counterexample needed
        if (s[k] == s[i])
            ++k;
        ret[i] = k;
    }

    return ret;
}

int main(int, char**)
{
    const std::string text{"siusiasiusikusiaksiusiakusia"};
    const std::string pat{"siusiak"};

    const auto pat_size = int(pat.size());
    const auto combined = pat + '$' + text;
    auto f = kmp(combined);
    const auto it = std::find(f.begin(), f.end(), pat_size);
    if (it == f.end())
        std::cout << "NOT FOUND\n";
    else {
        const auto off = it - f.begin();
        assert(pat == text.substr(off - 2 * pat_size, pat_size));
        std::cout << text << '\n';
        std::cout << std::setw(off - pat_size) << pat << std::endl;
    }
    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 kmp.cpp -o kmp.exe

*/

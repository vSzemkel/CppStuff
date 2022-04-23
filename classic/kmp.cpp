
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
            k = ret[k - 1]; // can't be ret[k] - counterexample: "aaab$aaaccc" inf-loops
        if (s[k] == s[i])
            ++k;
        ret[i] = k;
    }

    return ret;
}

/**
 * @brief Searches for prefixes of needle in the haystack string
 * 
 * @param s generalized string modelled as "needle$haystack"
 * @param n length of s
 * @return std::vector<int> ret[n] == k iff k-prefix of needle starts at haystack[n - 2 * size(needle)]
 */
template <typename T>
static std::vector<int> kmp_raw(const T* s, const size_t n) {
    std::vector<int> ret(n);
    for (int k = 0, i = 1; i < n; ++i) { // k is lenght of already found proper (non identity) bound 
        while (k > 0 && s[k] != s[i])
            k = ret[k - 1];
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
    const auto findings = kmp(combined);
    const auto it = std::find(findings.begin(), findings.end(), pat_size);
    if (it == findings.end())
        std::cout << "NOT FOUND\n";
    else {
        const auto off = it - findings.begin();
        assert(pat == text.substr(off - 2 * pat_size, pat_size));
        std::cout << text << '\n';
        std::cout << std::setw(off - pat_size) << pat;
    }

    std::cout << "\nPASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 kmp.cpp -o kmp.exe

*/


#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

// Knuth Morris Pratt search algorithm


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
        std::cout << text << '\n';
        std::cout << std::setw((it - f.begin()) - pat_size) << pat << std::endl;
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 kmp.cpp -o kmp.exe

*/

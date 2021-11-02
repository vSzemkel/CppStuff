
#include <iostream>
#include <span>
#include <string>
#include <string_view>
#include <vector>

// Longest common subsequence

template<typename T>
std::vector<T> LCS(std::span<T> t1, std::span<T> t2) { // N(rc)
    const int rows = int(t1.size()) + 1;
    const int cols = int(t2.size()) + 1;
    std::vector<std::vector<int>> lcs(rows, std::vector<int>(cols));
    for (int r = 1; r < rows; ++r)
        for (int c = 1; c < cols; ++c)
            if (t1[r - 1] == t2[c - 1])
                lcs[r][c] = lcs[r - 1][c - 1] + 1;
            else
                lcs[r][c] = std::max(lcs[r][c - 1], lcs[r - 1][c]);

    std::vector<T> ret;
    int r{rows - 1}, c{cols - 1};
    ret.reserve(lcs[r][c]);
    while (lcs[r][c] > 0) {
        if (lcs[r][c] == lcs[r - 1][c - 1] + 1) {
            --r; --c;
            ret.push_back(t1[r]);
        } else if (lcs[r][c] == lcs[r - 1][c])
            --r;
        else
            --c;
    }

    std::reverse(ret.begin(), ret.end());
    return ret;
}

int main(int, char**)
{
    std::string text1 = "----C-OG---N-IT--I-V-E- DI------S-SON--A-NC--E----";
    std::string text2 = "CO==GN=IT====IVE DI========S=S=O=NA=NC======E==";
    const auto foundc = LCS<char>(text1, text2);
    std::cout << std::string_view{foundc.data(), foundc.size()} << '\n';

    std::vector<int> vec1 = {0, 0, 0, 1, 0, 2, 0, 0, 3, 0, 4, 0, 0, 0, 0, 5, 0, 0};
    std::vector<int> vec2 = {1, 9, 9, 9, 2, 9, 3, 4, 9, 9, 9, 5, 9, 9, 9, 9, 9, 9};
    const auto foundi = LCS<int>(vec1, vec2);
    for (const int n : foundi)
        std::cout << n << ' ';
    std::cout << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 longest_common_subsequence.cpp -o longest_common_subsequence.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++20 -fsanitize=address longest_common_subsequence.cpp -o longest_common_subsequence

*/

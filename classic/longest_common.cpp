
#include <iostream>
#include <span>
#include <string>
#include <string_view>
#include <vector>

// Longest common consecutivesubsequence

template<typename T>
std::span<T> LCCS(std::span<T> t1, std::span<T> t2) {
    const int rows = int(t1.size()) + 1;
    const int cols = int(t2.size()) + 1;
    std::span<T> ret{t1.data(), 0};
    std::vector<std::vector<int>> lccs(rows, std::vector<int>(cols));
    for (int r = 1; r < rows; ++r)
        for (int c = 1; c < cols; ++c)
            if (t1[r - 1] == t2[c - 1]) {
                const auto len = lccs[r - 1][c - 1] + 1;
                lccs[r][c] = len;
                if (len > int(ret.size()))
                    ret = std::span<T>(t1.data() + r - len, len);
            }

    return ret;
}

int main(int, char**)
{
    std::string text1 = "DAKRNSXRZKDTMTJCGSFAAVAQNSIUSIAKEKFIAAWQHCF";
    std::string text2 = "KUTGAR2BXHI8SKFSIUSIAKVEKFI5AAWQHPCFISNZVGQIRFMS";
    const auto foundc = LCCS<char>(text1, text2);
    std::cout << std::string_view{foundc.data(), foundc.size()} << '\n';

    std::vector<int> vec1 = {2,7,1,2,3,4,5,3,8,9,23,56,8,3,1,4};
    std::vector<int> vec2 = {2,4,3,1,9,1,2,3,4,5,11,56,8,7,1,5};
    const auto foundi = LCCS<int>(vec1, vec2);
    for (const int n : foundi)
        std::cout << n << ' ';
    std::cout << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 longest_common.cpp -o longest_common.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++20 -fsanitize=address longest_common.cpp -o longest_common.o

*/

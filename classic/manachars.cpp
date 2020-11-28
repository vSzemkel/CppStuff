
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// Manachars algorithm
// For a given string find the longest palindrome substring


// case definition
const std::string text = "abccabcbcbabcbaabcabccbacbaabbcaabcacc";

// palindrome substring center can be on any character or in between any two
std::string expand()
{
    std::string ret(2 * text.size() + 3, '#');
    ret[0] = '$';
    ret.back() = '&';
    for (int i = 0; i < text.size(); ++i)
        ret[2 * (i + 1)] = text[i];
    return ret;
}

int main(int argc, char* argv[])
{
    int c{}, r{};
    std::string q = expand();
    auto p = std::vector<int>(q.size(), 0);
    // compute palindrome substrings map
    for (int i = 1; i < q.size() - 1; ++i) {
        if (i < r) {
            const auto mirror = c - (i - c);
            p[i] = std::min(p[mirror], r - i);
        }

        while (q[i - p[i] - 1] == q[i + p[i] + 1])
            p[i]++;

        if (i + p[i] > r) {
            r = i + p[i];
            c = i;
        }
    }
    // find the result
    const auto it = std::max_element(p.begin(), p.end());
    auto pos = (it - p.begin() - *it - 1) / 2;
    auto len = *it;
    // present result
    printf("The longest palindrome substring in %s has length of %i\n", text.c_str(), len);
    printf("\t        %s\n", text.c_str());
    printf("\t[%lli:%lli] %*s\n", pos, pos + len, static_cast<int>(pos + len), text.substr(pos, len).c_str());
    return 0;
}

/*    clang++.exe -Wall -g -std=c++17 manachars.cpp -o manachars.exe

Output:

The longest palindrome substring in abccabcbcbabcbaabcabccbacbaabbcaabcacc has length of 16
	        abccabcbcbabcbaabcabccbacbaabbcaabcacc
	[13:29]              baabcabccbacbaab

*/
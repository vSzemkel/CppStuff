
#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

// Manacher's algorithm
// For a given string find the longest palindrome substring or check for palindromic substring in constant time
// see: \kickstart\2022\matching_palindrome.cpp

class manacher_t {
  public:
    manacher_t(const std::string& s) {
        expand(s);
        manacher();
    }

    bool is_palindrome(const int begin, const int end) const { // [begin, end)
        return _radius[begin + end + 1] >= end - begin;
    }

    std::pair<int, int> longest() { // {offset, length}
        const auto it = std::max_element(_radius.begin(), _radius.end());
        auto pos = (it - _radius.begin() - *it - 1) / 2;
        auto len = *it;
        return {pos, len};
    }

  private:
    void expand(const std::string& s) {
        const int size = int(s.size());
        _exp_size = 2 * size + 3;
        _expanded.resize(_exp_size, '#');
        _expanded[0] = '$';
        _expanded.back() = '&';
        for (int i = 0; i < size; ++i)
            _expanded[2 * (i + 1)] = s[i];
    }

    void manacher() {
        int c{}, r{};
        _radius.resize(_exp_size, 0);
        // compute palindrome substrings map
        for (int i = 1; i < _exp_size - 1; ++i) {
            if (i < r) {
                const auto mirror = c - (i - c);
                _radius[i] = std::min(_radius[mirror], r - i);
            }

            while (_expanded[i - _radius[i] - 1] == _expanded[i + _radius[i] + 1])
                ++_radius[i];

            if (i + _radius[i] > r) {
                r = i + _radius[i];
                c = i;
            }
        }
    }

    int _exp_size;
    std::string _expanded;
    std::vector<int> _radius;
};

// case definition
const std::string text = "abccabcbcbabcbaabcabccbacbaabbcaabcacc";

int main(int argc, char* argv[])
{
    manacher_t m{text};
    const auto longest = m.longest();
    assert(m.is_palindrome(longest.first, longest.first + longest.second));

    // present result
    std::cout << text << '\n';
    std::cout << std::string(longest.first, ' ') << text.substr(longest.first, longest.second) << '\n';
    std::cout << "The longest palindrome substring of a length " << longest.second << " is subrange [" << longest.first << ':' << longest.first + longest.second << ")\n";
    return 0;
}

/*    clang++.exe -Wall -g -std=c++17 manachars.cpp -o manachars.exe

Output:

The longest palindrome substring in abccabcbcbabcbaabcabccbacbaabbcaabcacc has length of 16
	        abccabcbcbabcbaabcabccbacbaabbcaabcacc
	[13:29]              baabcabccbacbaab

*/
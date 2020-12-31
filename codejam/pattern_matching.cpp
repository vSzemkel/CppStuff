
#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <string_view>
#include <vector>

// Pattern Matching
// https://codingcompetitions.withgoogle.com/codejam/round/000000000019fd74/00000000002b3034

constexpr char star = '*';
constexpr int g_longest_ret_size = 10000;
const std::string not_found(1, star);

std::string merge_ordered(const std::string& left, const std::string& right)
{
    for (size_t i = 0; i < left.size(); ++i)
        if (right.find(left.substr(i)) == 0)
            return left.substr(0, i) + right;

    return left + right;
}

std::string merge(const std::string& left, const std::string& right)
{
    const auto lr = merge_ordered(left, right);
    const auto rl = merge_ordered(right, left);
    return (rl.size() < lr.size()) ? rl : lr;
}

std::string solve() {
    int size;
    std::cin >> size;
    std::vector<std::string> input(size);
    for (int i = 0; i < size; ++i)
        std::cin >> input[i];

    std::string ret_middle;
    std::vector<std::string_view> left, right;
    for (auto& s : input) {
        const auto left_star = s.find(star);
        const auto right_star = s.rfind(star);
        assert(left_star != std::string::npos); // task specification
        left.emplace_back(s.data(), left_star);
        right.emplace_back(s.data() + right_star + 1);
        if (left_star < right_star) { // 2+ stars
            size_t off{0};
            const std::string_view mid(s.data() + left_star + 1, right_star - left_star - 1);
            while (true) {
                const size_t pos = mid.find(star, off);
                if (pos == std::string::npos) break;
                ret_middle += mid.substr(off, pos - off);
                off = pos + 1;
            }

            if (off > 0)
                ret_middle += mid.substr(off);
            else if (ret_middle.find(mid) == std::string::npos)
                ret_middle = merge(ret_middle, std::string(mid));
        }
    }
    const auto ret_left = *std::max_element(left.begin(), left.end(), [](const std::string_view& s1, const std::string_view& s2){ return s1.size() < s2.size(); });
    if (!std::all_of(left.begin(), left.end(), [&ret_left](const std::string_view& s){ return ret_left.find(s) == 0; }))
        return not_found;
    const auto ret_right = *std::max_element(right.begin(), right.end(), [](const std::string_view& s1, const std::string_view& s2){ return s1.size() < s2.size(); });
    if (!std::all_of(right.begin(), right.end(), [&ret_right](const std::string_view& s){ return ret_right.rfind(s) + s.size() == ret_right.size(); }))
        return not_found;

    ret_middle.insert(0, ret_left);
    ret_middle.append(ret_right);
    return ret_middle.size() > g_longest_ret_size ? not_found : ret_middle;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": " << solve() << "\n";
    }
}

/*
cls && clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 pattern_matching.cpp -o pattern_matching.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 pattern_matching.cpp -o pattern_matching.o
pattern_matching.exe < pattern_matching.in

Input:

3
5
ALA*M*OTA
AL*MA*KOTA
A*AKOTA
AL*MAKO*TA
ALAM*K*A
5
E*CONUTS
EL*COCONUTS
E*OCONUTS
ELEF*CONUTS
EL*S
2
*XZ
*XYZ

Output:

Case #1: ALAMMAKOAKOTA
Case #2: ELEFCOCONUTS
Case #3: *

*/

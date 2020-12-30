
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

// Pattern Matching
// https://codingcompetitions.withgoogle.com/codejam/round/000000000019fd74/00000000002b3034

constexpr char star = '*';
constexpr int g_longest_ret_size = 10000;
const std::string not_found(1, star);

std::string solve() {
    int size;
    std::cin >> size;
    std::vector<std::string> input(size);
    for (int i = 0; i < size; ++i)
        std::cin >> input[i];

    // ret_left*ret_middle*ret_right
    std::string ret_left, ret_middle, ret_right;
    while (!input.empty()) {
        std::vector<std::string> left, middle, right, next_input;
        for (auto& s : input) {
            const auto left_star = s.find(star);
            const auto right_star = s.rfind(star);
            if (left_star != std::string::npos) { // 1+ star
                left.push_back(s.substr(0, left_star));
                right.push_back(s.substr(right_star + 1));
                if (left_star < right_star) { // 2+ stars
                    const auto mid = s.substr(left_star + 1, right_star - left_star - 1);
                    const auto third_star = s.find(star, left_star + 1);
                    if (third_star == right_star)
                        middle.push_back(mid);
                    else
                        next_input.push_back(mid);
                }
            } else 
                middle.push_back(s);
        }
        const auto max_left = *std::max_element(left.begin(), left.end(), [](const std::string& s1, const std::string& s2){ return s1.size() < s2.size(); });
        if (!std::all_of(left.begin(), left.end(), [&max_left](const std::string& s){ return max_left.find(s) == 0; }))
            return not_found;
        const auto max_right = *std::max_element(right.begin(), right.end(), [](const std::string& s1, const std::string& s2){ return s1.size() < s2.size(); });
        if (!std::all_of(right.begin(), right.end(), [&max_right](const std::string& s){ return max_right.rfind(s) + s.size() == max_right.size(); }))
            return not_found;

        ret_left = ret_left + max_left;
        ret_right = max_right + ret_right;
        input = next_input;
        if (!middle.empty()) {
            if (input.empty()) {
                ret_middle = *std::max_element(middle.begin(), middle.end(), [](const std::string& s1, const std::string& s2){ return s1.size() < s2.size(); });
                for (const auto& m : middle)
                    if (ret_middle.find(m) == std::string::npos)
                        return not_found;

                for (size_t i = 0; i < ret_left.size(); ++i)
                    if (ret_middle.find(ret_left.substr(i)) == 0) {
                        ret_middle = ret_middle.substr(ret_left.size() - i);
                        break;
                    }
                for (size_t i = 0; i < ret_middle.size(); ++i)
                    if (ret_right.find(ret_middle.substr(i)) == 0) {
                        ret_middle.resize(i);
                        break;
                    }
            } else
                input.insert(input.end(), middle.begin(), middle.end());
        }
    }

    ret_left += ret_middle + ret_right;
    return ret_left.size() > g_longest_ret_size ? not_found : ret_left;
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

Case #1: ALAMAKOTA
Case #2: ELEFCOCONUTS
Case #3: *

*/

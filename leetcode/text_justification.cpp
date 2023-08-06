
#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <filesystem>
#include <fstream>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <vector>

// Text Justification
// https://leetcode.com/problems/text-justification/?envType=study-plan-v2&envId=top-interview-150

constexpr char SPACE = '.';


// TODO: output the last line
// TODO: correct spacing
static void solve() {
    size_t N, W;
    std::cin >> N >> W;
    size_t current_line_size{};
    std::string current_word;
    std::vector<std::string> current_line, lines;
    for (size_t i = 0; i < N; ++i) {
        std::cin >> current_word;
        const auto current_word_size = current_word.size();
        if (current_line_size + current_word_size > W) {
            const auto spacing_count = current_line.size() - 1;
            const auto padding = W - current_line_size + 1;
            const auto reminder = padding % spacing_count;
            const std::string everyone(padding / spacing_count, SPACE);

            std::stringstream ss;
            for (size_t i = 0; i <= spacing_count; ++i) {
                ss << current_line[i] << everyone;
                if (i < spacing_count)
                    ss << SPACE;
                if (i < reminder)
                    ss << SPACE;
            }
            ss << '\n';
            lines.push_back(ss.str());
            current_line.clear();
            current_line_size = 0;
        }

        current_line.push_back(current_word);
        current_line_size += current_word_size + 1;
    }
    for (auto& s : lines)
        std::cout << '\n' << s ;

    //return lines;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 text_justification.cpp -o text_justification.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address text_justification.cpp -o text_justification

Run:
text_justification.exe < text_justification.in

Input:


Output:


*/

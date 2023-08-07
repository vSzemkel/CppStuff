
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
    int N, W;
    std::cin >> N >> W;
    int current_line_size{};
    std::string current_word;
    std::vector<std::string> current_line, lines;
    for (int i = 0; i < N; ++i) {
        std::cin >> current_word;
        const auto current_word_size = int(current_word.size());
        const auto spacing_count = int(current_line.size()) - 1;
        if (current_line_size + spacing_count + 1 + current_word_size > W) {
            const auto spaces = W - current_line_size;
            const auto padding = spaces / spacing_count;
            const auto reminder = spaces % spacing_count;
            const std::string everyone(padding, SPACE);

            std::stringstream ss;
            for (int i = 0; i < spacing_count; ++i) {
                ss << current_line[i] << everyone;
                if (i < reminder)
                    ss << SPACE;
            }
            if (spacing_count > 0)
                ss << current_line.back();
            lines.push_back(ss.str());
            current_line.clear();
            current_line_size = 0;
        }

        current_line.push_back(current_word);
        current_line_size += current_word_size;
    }

    if (!current_line.empty()) {
        std::stringstream ss;
        for (auto& s : current_line)
            ss << s << SPACE;
        const int padding = W - current_line_size - int(current_line.size());
        if (padding > 0)
            ss << std::string(padding, SPACE);
        lines.push_back(ss.str());
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

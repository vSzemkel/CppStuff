
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

// Text Justification
// https://leetcode.com/problems/text-justification/?envType=study-plan-v2&envId=top-interview-150

constexpr char SPACE = '.';

static void solve() {
    int N, W;
    std::cin >> N >> W;

    std::stringstream ss;
    std::string current_word, blank(W, SPACE);
    int current_line_size{}, spacing_count{-1};
    std::vector<std::string> current_line, lines;

    for (int i = 0; i < N; ++i) {
        std::cin >> current_word;
        const auto current_word_size = int(current_word.size());
        assert(current_word_size <= W);
        
        if (current_line_size + spacing_count + 1 + current_word_size > W && !current_line.empty()) {
            const auto spaces = W - current_line_size;
            if (spacing_count > 0) {
                const auto padding = spaces / spacing_count;
                const auto reminder = spaces % spacing_count;
                const auto everyone = std::string_view(blank.data(), padding);

                for (int i = 0; i < spacing_count; ++i) {
                    ss << current_line[i] << everyone;
                    if (i < reminder)
                        ss << SPACE;
                }
            }

            if (current_line.size() == 1)
                current_line.back().append(std::string_view(blank.data(), W - current_line_size));
            ss << current_line.back();
            lines.push_back(ss.str());
            ss.str({});
            current_line.clear();
            spacing_count = -1;
            current_line_size = 0;
        }

        ++spacing_count;
        current_line.push_back(current_word);
        current_line_size += current_word_size;
    }

    if (!current_line.empty()) {
        for (auto& s : current_line)
            ss << s << SPACE;
        const int padding = W - current_line_size - int(current_line.size());
        if (padding > 0)
            ss << std::string_view(blank.data(), padding);
        lines.push_back(ss.str());
        if (padding < 0)
            lines.back().resize(W);
    }

    for (auto& s : lines)
        std::cout << '\n' << s ;
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

1
7 20
333 55555 55555 22222222222222222222 999999999 0000000000 1

Output:

333....55555...55555
22222222222222222222
999999999.0000000000
1...................

*/


#include <algorithm>
#include <array>
#include <assert.h>
#include <bitset>
#include <cmath>
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
#include <stdlib.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <vector>

// Rectangles in Grid
// Maximal square, maximal rectangle, largest in histogram


constexpr const int R = 10 ,C = 20;
const std::string grid =
"01000110000000000000"
"01111111000111100010"
"00011111000000111010"
"00011111000111100010"
"00111111111111110000"
"00111111111111110100"
"00111111111111110011"
"00011000001100000000"
"11111001111111111111"
"01110000000111111111";

static void largest_rectangle() {
    bool augmented{true};
    int ans_len{0}, ans_r, ans_c;
    const int max = std::min(R, C);
    std::vector<std::vector<bool>> dp(R, std::vector<bool>(C));
    for (int r = 0; r < R - 1; ++r)
        for (int c = 0; c < C - 1; ++c)
            if (grid[r * C + c] == '1') {
                dp[r][c] = true;
                if (ans_len < 1) {
                    ans_len = 1;
                    ans_r = r; ans_c = c;
                }
            }

    for (int w = 2; augmented && w <= max; ++w) {
        augmented = false;
        for (int r = 0; r < R - w; ++r)
            for (int c = 0; c < C - w; ++c) {
                const bool found = dp[r][c] && dp[r][c + 1] && dp[r + 1][c] && dp[r + 1][c + 1];
                if (found && ans_len < w) {
                    ans_len = w;
                    ans_r = r; ans_c = c;
                    augmented = true;
                }
                dp[r][c] = found;
            }
    }

    std::cout << "Maximal rectangle has size " << ans_len << " ans starts in [" << ans_r + 1 << ", " << ans_c + 1 << "]\n";
}

int main(int, char**)
{
    largest_rectangle();
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 rectangles_in_grid.cpp -o rectangles_in_grid.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address rectangles_in_grid.cpp -o rectangles_in_grid

Output:

Maximal rectangle has size 5 ans starts in [2, 4]

*/

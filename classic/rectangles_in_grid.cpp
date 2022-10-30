
#include <iostream>
#include <cassert>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

// Rectangles in Grid
// Maximal square, maximal rectangle, largest in histogram


constexpr const int R = 10, C = 20;
const std::string grid =
"01000110000000000000"
"01111111000111100010"
"00011111000000111010"
"00011111000111100010"
"00111111111111111000"
"00111111111111110100"
"00111111111111110011"
"00011000001100011000"
"11111001111111111110"
"01110000000111111111";

static void largest_square() {
    int ans_len{0}, ans_r, ans_c;
    const int max = std::min(R, C);
    std::vector<std::vector<bool>> dp(R, std::vector<bool>(C));
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c)
            if (grid[r * C + c] == '1') {
                dp[r][c] = true;
                if (ans_len < 1) {
                    ans_len = 1;
                    ans_r = r; ans_c = c;
                }
            }

    bool augmented = ans_len > 0;
    for (int w = 2; augmented && w <= max; ++w) {
        augmented = false;
        for (int r = 0; r < R - w + 1; ++r)
            for (int c = 0; c < C - w + 1; ++c) {
                const bool found = dp[r][c] && dp[r][c + 1] && dp[r + 1][c] && dp[r + 1][c + 1];
                if (found && ans_len < w) {
                    ans_len = w;
                    ans_r = r; ans_c = c;
                    augmented = true;
                }
                dp[r][c] = found;
            }
    }

    std::cout << "Maximal square has size " << ans_len << " and starts in [" << ans_r + 1 << ", " << ans_c + 1 << "]\n";
}

using histrest_t = std::tuple<int, int, int, int>; // {area, left, heigth, right}

static histrest_t max_rect_in_histogram(const std::vector<int>& histogram) {
    assert(histogram.back() == 0);
    const int HSZ = int(histogram.size());
    histrest_t ret{-1, 0, 0, 0};
    std::vector<std::pair<int, int>> stack; // {left, heigth}
    for (int i = 0; i < HSZ; ++i) {
        int left = i;
        const int val = histogram[i];
        while (!stack.empty()) {
            const int heigth = stack.back().second;
            if (heigth >= val) {
                left = stack.back().first;
                stack.pop_back();
                histrest_t can{heigth * (i - left), left, heigth, i};
                if (ret < can)
                    ret = can;
            } else
                break;
        }
        stack.emplace_back(left, val);
    }

    return ret;
}

static void largest_rectangle() {
    int ans_area{0}, ans_r, ans_c, ans_w, ans_h;
    std::vector<int> histogram(C + 1);
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c) {
            if (grid[r * C + c] == '1')
                ++histogram[c];
            else
                histogram[c] = 0;
            const auto can = max_rect_in_histogram(histogram);
            if (ans_area < std::get<0>(can)) {
                ans_area = std::get<0>(can);
                ans_c = std::get<1>(can);
                ans_h = std::get<2>(can);
                ans_w = std::get<3>(can) - ans_c;
                ans_r = r - ans_h + 1;
            }
        }

    std::cout << "Maximal rectangle has area " << ans_area << ", with width=" << ans_w << " and heigth=" << ans_h << " and starts in [" << ans_r + 1 << ", " << ans_c + 1 << "]\n";
}

int main(int, char**)
{
    const std::vector<int> histogram = {2, 2, 2, 4, 5, 5, 3, 3, 0};
    const auto mrh = max_rect_in_histogram(histogram);
    std::cout << "Largest rectangle in histogram has area of " << std::get<0>(mrh) << ", heigth of " << std::get<2>(mrh) << " and starts from " << std::get<1>(mrh) << '\n';
    largest_square();
    largest_rectangle();
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 rectangles_in_grid.cpp -o rectangles_in_grid.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address rectangles_in_grid.cpp -o rectangles_in_grid

Output:

Largest rectangle in histogram has area of 16, heigth of 2 and starts from 0
Maximal square has size 5 and starts in [2, 4]
Maximal rectangle has area 42, with width=14 and heigth=3 and starts in [5, 3]

*/

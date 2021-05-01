
#include <algorithm>
#include <iostream>
#include <vector>

// Digit Blocks
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000435baf/00000000007ae37b

int64_t P;
int N, B;

static void solve() {
    const int total = N * B;
    std::vector<int> heights(N);
    for (int i = 0; i < total; ++i) {
        int t, d; std::cin >> d;
        if (d == 9) // put 9 on top if it is possible
            for (t = 0; t < N; ++t)
                if (heights[t] == B - 1) {
                    ++heights[t];
                    goto decided;
                }

        if (d > 6) // put large number on second to top if it is possible
            for (t = 0; t < N; ++t)
                if (heights[t] == B - 2) {
                    ++heights[t];
                    goto decided;
                }

        for (t = 0; t < N; ++t) // coalesce to build B - 2 height tower
            if (heights[t] < B - 2) {
                ++heights[t];
                goto decided;
            }

        { // put on shortest tower
            const auto it = std::min_element(heights.begin(), heights.end());
            t = it - heights.begin();
            ++heights[t];
        }
decided:
        std::cout << t + 1 << std::endl;
    }
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases >> N >> B >> P;
    for (int g = 1; g <= no_of_cases; ++g) {
        solve();
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 digit_blocks.cpp -o digit_blocks.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address digit_blocks.cpp -o digit_blocks.o

Run:
py.exe interactive_runner.py py.exe digit_blocks_testing_tool.py 1 -- digit_blocks.exe

*/


#include <cassert>
#include <iostream>
#include <tuple>
#include <utility>
#include <vector>

// Circuit Board
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050ff2/0000000000150aae#analysis

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

static void solve() {
    int R, C, K;
    std::cin >> R >> C >> K;

    std::vector<std::vector<int>> data(R, std::vector<int>(C));
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c)
            std::cin >> data[r][c];

    std::vector<std::vector<int>> histogram(C, std::vector<int>(R + 1));
    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c) {
            const int v = data[r][c];
            int mi{v}, ma{v};
            histogram[c][r] = 1;
            for (int d = c - 1; ~d; --d) {
                const int cur = data[r][d];
                mi = std::min(mi, cur);
                ma = std::max(ma, cur);
                if (ma - mi > K)
                    break;
                ++histogram[d][r];
            }
        }
    }

    int ans_area{0};
    for (int r = 0; r < C; ++r) {
        const auto can = max_rect_in_histogram(histogram[r]);
        ans_area = std::max(ans_area, std::get<0>(can));
    }

    std::cout << ans_area;
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 circuit_board.cpp -o circuit_board.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address circuit_board.cpp -o circuit_board

Run:
circuit_board.exe < circuit_board.in

Input:

3
1 4 2
3 1 3 3
3 3 2
0 5 0
8 12 3
7 10 1
4 4 8
20 10 20 10
10 4 5 20
20 5 4 10
10 20 10 20

Output:

Case #1: 4
Case #2: 3
Case #3: 4

*/

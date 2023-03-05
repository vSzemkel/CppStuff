
#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

// Diagonal Puzzle
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050edd/00000000001a2835

constexpr const int INF = 1e09;
constexpr const int LONGEST = 1;
constexpr const int SEC_LONGEST = 2;

/**
 * @Observation: setting longest and second to longest diagonal determines other moves
 */
static void solve() {
    int N;
    std::cin >> N;
    const std::string pat(N, '#');
    std::vector<std::string> board(N);
    for (auto& r : board)
        std::cin >> r;

    int ans{INF};
    const auto turn = [](char& c){ c = (c == '.' ? '#' : '.'); };
    for (int m = 0; m < 4; ++m) {
        // arrange selected diagonals
        int cur{0};
        auto copy = board;
        if (m & LONGEST) {
            ++cur;
            for (int z = N, r = N - 1, c = 0; z; --z, --r, ++c)
                turn(copy[r][c]);
        }
        if (m & SEC_LONGEST) {
            ++cur;
            for (int z = N - 1, r = N - 1, c = 1; z; --z, --r, ++c)
                turn(copy[r][c]);
        }

        // for every orthogonal diag set if selected diagonal needs change
        for (int z = 2 * N - 1, p = 0, r = N - 1, c = 0; z; --z, p = 1 - p) {
            const int x = std::min(r, c);
            int s = r - x;
            int d = c - x;
            if (copy[r][c] == '.') {
                ++cur;
                while (s < N && d < N)
                    turn(copy[s++][d++]);
            }
            p ? --r : ++c;
        }

        // for every parallel diag set if selected diagonal needs change
        for (int r = 0; r < N; ++r)
            if (copy[r][0] == '.') {
                ++cur;
                for (int z = r + 1, s = r, d = 0; z; --z, --s, ++d)
                    turn(copy[s][d]);
            }
        for (int c = 1; c < N; ++c)
            if (copy[N - 1][c] == '.') {
                ++cur;
                for (int z = N - c, s = N - 1, d = c; z; --z, --s, ++d)
                    turn(copy[s][d]);
            }

        // validity check
        if (std::all_of(copy.begin(), copy.end(), [&pat](auto& r){ return r == pat; }))
            ans = std::min(ans, cur);
    }

    std::cout << ans;
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
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++17 diagonal_puzzle.cpp -o diagonal_puzzle.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address diagonal_puzzle.cpp -o diagonal_puzzle

Run:
diagonal_puzzle.exe < diagonal_puzzle.in

Input:

3
3
..#
#.#
#..
5
.####
#.###
##.##
###.#
#####
5
#.###
..###
##.#.
####.
##..#

Output:

Case #1: 3
Case #2: 2
Case #3: 6

*/

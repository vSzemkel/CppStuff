
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>

// Square Free
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000436142/0000000000813e1a#analysis


int R, C, MAXLEN;
std::vector<std::string> board;

static bool is_square_free() {
    std::vector<std::vector<std::pair<int, int>>> memo; // {count of '\' above, count of '/' above}
    memo.assign(R, std::vector<std::pair<int, int>>(C));

    for (int c = 0; c < C; ++c)
        memo[0][c] = (board[0][c] == '/') ? std::pair{0, 1} : std::pair{1, 0};
    for (int r = 1; r < R; ++r) {
        // update memo
        for (int c = 0; c < C; ++c) {
            const bool forward = board[r][c] == '/';
            int left = forward ? 0 : 1;
            int right = forward ? 1 : 0;
            if (left > 0 && c > 0 && board[r - 1][c - 1] != '/')
                left += memo[r - 1][c - 1].first;
            if (right > 0 && c < C - 1 && board[r - 1][c + 1] == '/')
                right += memo[r - 1][c + 1].second;
            memo[r][c] = {left, right};
        }
        // search for square
        const int bound = std::min(MAXLEN, (r + 1) / 2);
        for (int l = 1; l <= bound; ++l)
            for (int c = bound; c <= C - bound; ++c)
                if (board[r][c - 1] != '/' && board[r][c] == '/') {
                    const int len = std::min(memo[r][c - 1].first, memo[r][c].second);
                    for (int k = 1; k <= len; ++k)
                        if (memo[r - k][c - k].second >= k && memo[r - k][c + k - 1].first >= k)
                            return false;
                }
    }

    return true;
}

static void solve() {
    std::cin >> R >> C;
    MAXLEN = std::min(R, C);
    std::vector<int> rlimits(R);
    for (auto& r : rlimits) std::cin >> r;
    std::vector<int> climits(C);
    for (auto& c : climits) std::cin >> c;
    board.resize(R);

    //std::cout << std::boolalpha << is_square_free() << '\n';
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 square_free.cpp -o square_free.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address square_free.cpp -o square_free

Run:
square_free.exe < square_free.in

Input:

4
4 4
3 2 3 3
3 3 2 3
2 3
1 1
1 1 1
2 3
1 2
1 1 1
3 3
2 0 2
2 0 2

Output:


*/


#include <algorithm>
#include <iomanip>
#include <iostream>
#include <optional>
#include <utility>
#include <tuple>
#include <vector>

// Square Free
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000436142/0000000000813e1a#analysis


int R, C, MAXLEN, SIZE;
std::vector<std::string> board;
std::vector<int> rlimits, climits, rsums, csums;

static std::optional<std::tuple<int, int, int>> is_square_free() {
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
        for (int len = 1; len <= bound; ++len)
            for (int c = len; c <= C - len; ++c)
                if (memo[r][c - 1].first >= len && memo[r][c].second >= len
                 && memo[r - len][c - len].second >= len && memo[r - len][c + len - 1].first >= len)
                    return std::tuple{r, c, len};
    }

    return std::nullopt;
}

static bool generate(const int pos) {
    if (pos == SIZE) 
        return rsums == rlimits && csums == climits;

    const int r = pos / C;
    const int c = pos % C;

    if (rsums[r] < rlimits[r] && csums[c] < climits[c]) {
        ++rsums[r];
        ++csums[c];
        board[r][c] = '/';
        if (generate(pos + 1))
            return true;
        --rsums[r];
        --csums[c];
    } 
    
    board[r][c] = '\\';
    return generate(pos + 1);
}

static void solve() {
    std::cin >> R >> C;
    SIZE = R * C;
    MAXLEN = std::min(R, C);
    rlimits.assign(R, 0); rsums.assign(R, 0);
    for (auto& r : rlimits) std::cin >> r;
    climits.assign(C, 0); csums.assign(C, 0);
    for (auto& c : climits) std::cin >> c;
    board.assign(R, std::string(C, ' '));

    if (!generate(0)) {
        std::cout << "IMPOSSIBLE\n";
        return;
    }

    std::optional<std::tuple<int, int, int>> config;
    while ((config = is_square_free()) != std::nullopt) {
        const auto& cfg = config.value();
        const int r = std::get<0>(cfg);
        const int c = std::get<1>(cfg);
        const int u = r - 2 * std::get<2>(cfg) + 1;
        std::swap(board[r][c - 1], board[r][c]);
        std::swap(board[u][c - 1], board[u][c]);
    }

    std::cout << "POSSIBLE\n";
    for (const auto& s : board)
        std::cout << s << '\n';
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

Case #1: POSSIBLE
/\//
//\\
//\/
\///

Case #2: IMPOSSIBLE

Case #3: POSSIBLE
\\/
//\

Case #4: POSSIBLE
/\/
\\\
/\/

*/

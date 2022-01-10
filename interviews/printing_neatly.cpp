
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

// Printing Neatly
// The input text is a sequence of n words of lengths l1; l2; : : : ; ln, 
// measured in characters. We want to print this paragraph neatly on a number
// of lines that hold a maximum of L characters each. Our criterion of “neatness” 
// is as follows. If a given line contains words i through j,  where i <= j,
// and we leave exactly one space between words, the number of extra space 
// characters at the end of the line is M - j + i - SUM(l[i..j]), which must be
// nonnegative so that the words fit on the line. We wish to minimize the sum, 
// over all lines except the last, of the cubes of the numbers of extra space
// characters at the ends of lines

// Analysis
// x x x x x x x x x| 0
// xxxxxxx          | 10^3
// 
// x x x x x x      | 6^3
// x x x xxxxxxx    | 4^3
// 
// 1000 > 216 + 64 == 280

constexpr const int INF = 1e09;

std::vector<int> g_partial;

static void solve() {
    int L, N; std::cin >> L >> N;
    std::vector<int> words(N);
    for (auto& w : words)
        std::cin >> w;

    const auto range_sum = [](const int i, const int j){return g_partial[j + 1] - g_partial[i];};
    g_partial.resize(N + 1);
    std::partial_sum(words.begin(), words.end(), g_partial.begin() + 1);

    std::vector<int> dp(N, INF), memo(N);
    for (int start = N - 1; start >= 0; --start) {
        int extra_words_in_line{0};
        while (start + extra_words_in_line < N) {
            const int margin = L - extra_words_in_line - range_sum(start, start + extra_words_in_line);
            if (margin < 0) break; // line off limit
            const int next = start + extra_words_in_line + 1;
            int cost{0}; // last line not included in cost
            if (next < N)
                cost = margin * margin * margin + dp[next];
            if (cost < dp[start]) {
                dp[start] = cost;
                memo[start] = extra_words_in_line;
            }
            ++extra_words_in_line;
        }
    }

    std::cout << dp[0] << '\n';
    std::string line(L + 1, '#');
    line[L] = '|';
    for (int start = 0, end = 0; start < N; start = end + 1) {
        end = start + memo[start];
        if (end >= N) break;
        std::string l = line;
        int pos = words[start];
        for (int w = start + 1; w <= end; ++w) {
            l[pos++] = '.';
            pos += words[w];
        }
        for (int i = pos; i < L; ++i)
            l[i] = '.';
        std::cout << l << '\n';
    }
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve();
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 printing_neatly.cpp -o printing_neatly.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address printing_neatly.cpp -o printing_neatly

Run:
printing_neatly.exe < printing_neatly.in

Input:

2
17 10
1 1 1 1 1 1 1 1 1 7
40 20
4 2 2 7 5 4 1 2 9 8 5 11 3 6 3 3 9 5 4 7

Output:

Case #1: 0
#.#.#.#.#.#.#.#.#|
#######..........|
Case #2: 281
####.##.##.#######.#####.####.#.##......|
#########.########.#####.###########....|
###.######.###.###.#########.#####.####.|
#######.................................|

*/

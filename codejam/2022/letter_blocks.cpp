
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// Letter Blocks
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000877b42/0000000000afe6a1


constexpr int pct(int x) { return __builtin_popcount(x); } // # of bits set

int N;
int total; // chars bitmap
std::string result;
std::vector<bool> used;
std::vector<std::pair<std::string, int>> words; // {word, letters bitmap (A == 1)}

/**
 * @brief Checks if there is no interleaving in string
 * @return true if there are i < j < k such that s[i] == s[k] and s[i] != s[j]
 */
static bool is_interleaved(const std::string& s) {
    const int size = int(s.size());
    if (size < 2) return false;

    int changes{0};
    int bitmap = 1 << (s[0] - 'A');
    for (int i = 1; i < size; ++i) {
        bitmap |= 1 << (s[i] - 'A');
        if (s[i - 1] != s[i])
            ++changes;
    }

    return pct(bitmap) <= changes;
}

static bool solve_inner(const char c) {
    std::string ans;
    int ans_total{0};
    const auto ltr = 1 << (c - 'A');
    for (int i = 0; i < N; ++i) {
        if (used[i] || (words[i].second & ltr) == 0)
            continue;
        if (ans.empty()) {
            ans = words[i].first;
            ans_total |= words[i].second;
            used[i] = true;
            continue;
        } else {
            const auto cmn = pct(ans_total & words[i].second);
            if (cmn > 1)
                return false;
            if (cmn == 1) {
                if (ans.back() == words[i].first.front()) {
                    ans_total |= words[i].second;
                    ans += words[i].first;
                    used[i] = true;
                    continue;
                }
                if (ans.front() == words[i].first.back()) {
                    ans_total |= words[i].second;
                    ans = words[i].first + ans;
                    used[i] = true;
                    continue;
                }

                return false;
            }
        }
    }

    if (ans.empty())
        return true;
    if (ans.front() == c || solve_inner(ans.front())) {
        result += ans;
        if (ans.back() == c || solve_inner(ans.back())) {
            total &= ~ans_total;
            return true;
        }
    }

    return false;
}

static std::string solve() {
    std::cin >> N;
    total = 0;
    words.assign(N, {});
    used.assign(N, false);
    for (auto& w : words) {
        std::cin >> w.first;
        for (const char c : w.first) {
            const auto m = (1 << (c - 'A'));
            total |= m;
            w.second |= m;
        }
    }

    // check for invalid input words
    for (const auto& [w, l] : words)
        if (is_interleaved(w))
            return "IMPOSSIBLE";

    // uniform words should precede combined: AAAA before BAAA and AAC
    std::sort(words.begin(), words.end(), [](const auto& w1, const auto& w2){
        return w1.second < w2.second;
    });

    result.clear();
    while (total) {
        const char start = 'A' + __builtin_ctz(total);
        if (!solve_inner(start))
            return "IMPOSSIBLE";
    }

    if (is_interleaved(result))
        return "IMPOSSIBLE";

    return result;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": " << solve() << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 letter_blocks.cpp -o letter_blocks.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address letter_blocks.cpp -o letter_blocks

Run:
letter_blocks.exe < letter_blocks.in

Input:

8
5
CODE JAM MIC EEL ZZZZZ
6
CODE JAM MIC EEL ZZZZZ EEK
2
OY YO
2
HASH CODE
6
A AA BB A BA BB
2
CAT TAX
3
ABBB BC A
6
A BC C A B CCA

Output:

Case #1: JAMMICCODEEELZZZZZ
Case #2: IMPOSSIBLE
Case #3: IMPOSSIBLE
Case #4: IMPOSSIBLE
Case #5: BBBBBAAAAA
Case #6: IMPOSSIBLE
Case #7: AABBBBC
Case #8: BBCCCCAAA

*/

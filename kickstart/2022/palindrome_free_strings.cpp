
#include <iostream>
#include <unordered_set>
#include <string>
#include <vector>

// Palindrome Free Strings
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb33e/00000000009e762e

constexpr const int L = 5;

int N;
std::string S;

static bool is_palindrome(const std::string& s) {
    int l = 0, r = s.size() - 1;
    while (l < r)
        if (s[l++] != s[r--]) return false;
    return true;
}

static bool _solve() {
    if (N < L)
        return true;

    /**
     * @brief Prepare set of L-prefixes not containing '?', not palindromic
     */
    std::unordered_set<std::string> cache, tmp = {""}; // string of non-palindrome strings of length L, no '?'
    for (int i = 0; i < L; ++i) {
        cache = std::move(tmp);
        for (const auto& p : cache)
            if (S[i] == '?') {
                tmp.insert(p + '0');
                tmp.insert(p + '1');
            } else
                tmp.insert(p + S[i]);
    }
    cache.clear();
    for (const auto& c : tmp)
        if (!is_palindrome(c))
            cache.insert(c);

    /**
     * @brief cache stores all valid L-strings that could end on position pos - 1
     * Considering next character we check both L+1 string and its L-suffix
     */
    for (int pos = L; pos < N; ++pos) { // pos is last character of possible palindrome
        decltype(cache) tmp;
        for (const auto& p : cache) {
            const auto opt = (S[pos] == '?') ? std::vector{'0', '1'} : std::vector(1, S[pos]);
            for (const char t : opt) {
                const auto c = p + t;
                if (!is_palindrome(c)) {
                    const auto n = c.substr(1);
                    if (!is_palindrome(n))
                        tmp.insert(n);
                }
            }
        }

        if (tmp.empty())
            return false;

        cache = std::move(tmp);
    }

    return true;
}

static void solve() {
    std::cin >> N >> S;
    std::cout << (_solve() ? "POSSIBLE" : "IMPOSSIBLE");
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 palindrome_free_strings.cpp -o palindrome_free_strings.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address palindrome_free_strings.cpp -o palindrome_free_strings

Run:
py.exe interactive_runner.py py.exe palindrome_free_strings_testing_tool.py 1 -- palindrome_free_strings.exe
palindrome_free_strings.exe < palindrome_free_strings.in

Input:

14
110???0????1??
15
0?0?10?1?01??11

Output:

Case #1: IMPOSSIBLE
Case #2: POSSIBLE

*/

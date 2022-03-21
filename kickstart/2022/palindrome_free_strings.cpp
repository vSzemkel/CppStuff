
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

template <typename T>
static bool is_palindrome(const T v, const int size) {
    T l = 1, u = 1 << (size - 1);
    while (l < u) {
        if (bool(v & l) != bool(v & u)) return false;
        l <<= 1;
        u >>= 1;
    }
    return true;
}

static bool solve() { // real    0m1.588s
    std::cin >> N >> S;
    if (N < L)
        return true;

    /**
     * @brief Prepare set of L-prefixes not containing '?', not palindromic
     */
    constexpr const char mask5 = (1 << 5) - 1;
    constexpr const char mask6 = (1 << 6) - 1;
    std::unordered_set<char> cache, tmp = {0}; // representation of non-palindrome strings of length L, no '?'
    for (int i = 0; i < L; ++i) {
        cache = std::move(tmp);
        for (const char p : cache)
            if (S[i] == '?') {
                tmp.insert(mask5 &  (p << 1));
                tmp.insert(mask5 & ((p << 1) + 1));
            } else
                tmp.insert(mask5 & ((p << 1) + (S[i] - '0')));
    }
    cache.clear();
    for (const char c : tmp)
        if (!is_palindrome(c, 5))
            cache.insert(c);

    /**
     * @brief cache stores all valid L-strings that could end on position pos - 1
     * Considering next character we check both L+1 string and its L-suffix
     */
    for (int pos = L; pos < N; ++pos) { // pos is last character of possible palindrome
        decltype(cache) tmp;
        for (const auto& p : cache) {
            const auto opt = (S[pos] == '?') ? std::vector{0, 1} : std::vector(1, S[pos] - '0');
            for (const char t : opt) {
                char c = mask6 & ((p << 1) + t);
                if (!is_palindrome(c, 6)) {
                    c &= mask5;
                    if (!is_palindrome(c, 5))
                        tmp.insert(c);
                }
            }
        }

        if (tmp.empty())
            return false;

        cache = std::move(tmp);
    }

    return true;
}

static bool solve_string() { // real    0m2.682s
    std::cin >> N >> S;
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

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g)
        std::cout << "Case #" << g << ": " << (solve() ? "" : "IM") << "POSSIBLE\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 palindrome_free_strings.cpp -o palindrome_free_strings.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address palindrome_free_strings.cpp -o palindrome_free_strings

Run:
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


#include <iostream>
#include <format>
#include <string>
#include <vector>

// Substitution Cipher
// https://www.facebook.com/codingcompetitions/hacker-cup/2024/round-1/problems/D

static bool is_valid(const std::string& s)
{
    if (s.empty() || s.front() < '1' || '9' < s.front())
        return false;
    const int sz = int(s.size());
    for (int i = 1; i < sz; ++i) {
        auto c = s[i];
        if (c < '0' || '9' < c)
            return false;
        if (c == '0' && s[i - 1] != '1' && s[i - 1] != '2')
            return false;
    }

    return true;
}

static int64_t decodings_count(const std::string& s)
{
    if (!is_valid(s))
        return 0;

    const int sz = int(s.size());
    std::vector<int64_t> dp(sz + 1);
    dp[0] = 1;
    const auto add = [&](const int index, const int value) {
        auto& count = dp[index];
        count += value;
        count %= 998244353;
    };

    for (int i = 0; i < sz; ++i) {
        const auto count = dp[i];
        if (s[i] != '0')
            add(i + 1, count);
        if (i + 2 < sz + 1) {
            if (s[i] == '1')
                add(i + 2, count);
            else if (s[i] == '2' && s[i + 1] < '7')
                add(i + 2, count);
        }
    }

    return dp.back();
}

static int64_t solve()
{
    int K;
    std::string E;
    std::cin >> E >> K;

    std::string s("1111");
    return decodings_count(s);
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g)
        std::cout << std::format("Case #{}: {}\n", g, solve());
}

/*

Format:
clang-format -i substitution_cipher.cpp

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 substitution_cipher.cpp -o substitution_cipher.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address substitution_cipher.cpp -o substitution_cipher

Run:
substitution_cipher.exe < substitution_cipher.in

Input:

6
??2 3
135201 1
?35 2
1?0 2
1122 1
3???????????????????3 1337

Output:

Case #1: 122 3
Case #2: 135201 2
Case #3: 135 2
Case #4: 110 1
Case #5: 1122 5
Case #6: 322222222121221112223 10946

*/

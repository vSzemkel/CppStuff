
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

// I, O Bot
// https://codingcompetitions.withgoogle.com/codejam/round/00000000008778ec/0000000000b15167

int N, C;

/**
 * @Observation1: if rightmost two shapes differs then they should be collected in single trip (matched)
 * @Observation2: if rightmost two shapes are equal then we have 3 cases to consider (match them, match rightmost with rightmost different, leave unmatched)
 */
static int64_t solve_inner2(const std::vector<int>& type0, const std::vector<int>& type1) {
    if (type0.empty()) // type0.size() >= type1.size()
        return 0;
    const int s0 = int(type0.size());
    const int s1 = int(type1.size());
    std::vector<std::vector<int64_t>> dp(s0 + 1, std::vector<int64_t>(s1 + 1));
    dp[1][0] = 2 * type0[0];
    for (int i = 2; i <= s0; ++i)
        dp[i][0] = std::min(dp[i - 1][0], dp[i - 2][0] + C) + 2 * type0[i - 1]; // {unmatch, match(i-1, i)}
    if (type1.empty())
        return dp[s0][0];
    dp[0][1] = 2 * type1[0];
    for (int j = 2; j <= s1; ++j)
        dp[0][j] = std::min(dp[0][j - 1], dp[0][j - 2] + C) + 2 * type1[j - 1]; // {unmatch, match(i-1, i)}

    dp[1][1] = 2 * std::max(type0[0], type1[0]);
    for (int i = 2; i <= s0; ++i)
        dp[i][1] = std::min(dp[i - 1][0] + 2 * std::max(type0[i - 1], type1[0]), dp[i - 2][1] + C + 2 * type0[i - 1]);
    for (int j = 2; j <= s1; ++j)
        dp[1][j] = std::min(dp[0][j - 1] + 2 * std::max(type0[0], type1[j - 1]), dp[1][j - 2] + C + 2 * type1[j - 1]);

    for (int i = 2; i <= s0; ++i)
        for (int j = 2; j <= s1; ++j) {
            if (type0[i - 1] < type1[j - 1]) { // consider matches for type1[j]
                dp[i][j] = std::min(
                    dp[i][j - 2] +  2 * type1[j - 1] + C,
                    dp[i][j - 1] +  2 * type1[j - 1]
                );
            } else { // consider matches for type0[i]
                dp[i][j] = std::min(
                    dp[i - 2][j] +  2 * type0[i - 1] + C,
                    dp[i - 1][j] +  2 * type0[i - 1]
                );
            }

            dp[i][j] = std::min(dp[i][j], dp[i - 1][j - 1] + 2 * std::max(type0[i - 1], type1[j - 1]));
        }

    return dp[s0][s1];
}

static int64_t solve_inner(const std::vector<std::pair<int, bool>>& data) { // O(N2)
    std::vector<int> type0, type1;
    for (const auto& b : data)
        if (b.second)
            type1.push_back(b.first);
        else
            type0.push_back(b.first);

    std::sort(type0.begin(), type0.end());
    std::sort(type1.begin(), type1.end());
    if (type0.size() < type1.size())
        std::swap(type0, type1);

    return solve_inner2(type0, type1);
}

/**
 * @Observation3: rightmost shapes of different types should be matched
 */
static int64_t solve_inner3(std::vector<std::pair<int, bool>>& data) { // O(N) by tourist
    const int size = int(data.size());
    std::sort(data.begin(), data.end());
    std::vector<int> balance(size + 1);
    for (int i = 0; i < size; ++i) {
        data[i].first <<= 1; // for two way trip cost calculation
        balance[i + 1] = balance[i] + (data[i].second ? 1 : -1);
    }
    std::vector<std::vector<int64_t>> pref(2, std::vector<int64_t>(size + 1));
    for (int s = 0; s < 2; ++s)
        for (int i = 0; i < size; ++i)
            pref[s][i + 1] = pref[s][i] + (int(data[i].second) == s ? data[i].first : 0);

    std::vector<int64_t> dp(size + 1);
    std::unordered_map<int, int> mp; // last known position of arbitrary balance
    mp[0] = 0;
    for (int i = 1; i <= size; ++i) {
        dp[i] = dp[i - 1] + data[i - 1].first; // not matching case
        if (i > 1)
            dp[i] = std::min(dp[i], dp[i - 2] + data[i - 1].first + C); // case of last two are compatible

        const auto last = mp.find(balance[i]);
        if (last != mp.end()) { // case of last two are not compatible
            const int p = last->second;
            const int s = data[i - 1].second;
            dp[i] = std::min(dp[i], dp[p] + pref[s][i] - pref[s][p]);
        }
        mp[balance[i]] = i;
    }

    return dp[size];
}

static void solve() {
    std::cin >> N >> C;
    std::vector<std::pair<int, bool>> pos, neg;
    for (int z = N; z; --z) {
        int X, S; std::cin >> X >> S;
        if (X > 0)
            pos.emplace_back(X, S);
        else
            neg.emplace_back(-X, S);
    }

    std::cout << solve_inner3(pos) + solve_inner3(neg);
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 io_bot.cpp -o io_bot.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address io_bot.cpp -o io_bot

Run:
io_bot.exe < io_bot.in

Input:

4
5 0
3 0
6 0
8 0
10 1
15 1
5 10
3 0
6 0
8 0
10 1
15 1
5 1
3 0
6 0
8 0
10 1
15 1
2 0
1000000000 0
-1000000000 1

Output:

Case #1: 52
Case #2: 56
Case #3: 54
Case #4: 4000000000

*/

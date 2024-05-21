
#include <algorithm>
#include <array>
#include <iostream>
#include <ranges>
#include <unordered_map>
#include <vector>

// Best Time to Buy and Sell Stock with at most K transactions
// https://leetcode.com/problems/best-time-to-buy-and-sell-stock-iv

/**
 * We will analyze sequence of prices from the end.
 * Every day we can or can not buy a stock - depending of previous actions
 * The decision is evaluated in context of possible future continuation
*/
int maxProfit(int k, std::vector<int>& prices) {
    using dp_t = std::vector<std::array<int, 2>>;
    dp_t current(k + 1), future(k + 1);

    for (const int price : prices | std::views::reverse) {
        for (int limit = 1; limit <= k; ++limit)
            for (int can_buy = 0; can_buy < 2; ++can_buy) {
                auto& profit = current[limit][can_buy];
                // max(act, wait)
                if (can_buy)
                    profit = std::max(-price + future[limit][0], future[limit][1]);
                else
                    profit = std::max(price + future[limit - 1][1], future[limit][0]);
            }

        std::swap(future, current);
    }

    return future[k][1];
}

/**
 * Slow implementation is direct generalization of the approach taken in best-time-to-buy-and-sell-stock-iii
 * It is not a crap but there is much better algorithm
*/
std::unordered_map<int, int> memo; // hash -> profit
std::vector<int> prices;

int maxProfitSlowInternal(const int k, const int begin, const int end) {
    const auto sz = end - begin;
    if (sz < 2)
        return 0;
    const auto hash = (((begin << 10) + end) << 10) + k;
    const auto found = memo.find(hash);
    if (found != memo.end())
        return found->second;

    std::vector<int> best_left(sz + 1); // best[i] is best of i left most elements
    int ans{}, min{prices[begin]};
    for (int z = sz - 1, i = begin + 1, b = 1; z; ++i, ++b, --z) {
        best_left[b + 1] = std::max(best_left[b], prices[i] - min);
        min = std::min(min, prices[i]);
    }

    ans = best_left[sz];
    int max = prices.back();
    if (k > 1)
        for (int i = end - 2, b = i - begin; i > begin + 1; --i, --b) { // left of size >= 2 to be effective
            if (k > 2) {
                const auto nk = std::min(k - 1, (end - i) / 2);
                ans = std::max(ans, best_left[b] + maxProfitSlowInternal(nk, i, end));
            } else
                ans = std::max(ans, best_left[b] + max - prices[i]);
            max = std::max(max, prices[i]);
        }

    return memo[hash] = ans;
}

int maxProfitSlow(int k, std::vector<int>& _prices) {
    prices = _prices;
    memo.reserve(k * k * k);
    return maxProfitSlowInternal(k, 0, int(prices.size()));
}

static void io_handler()
{
    int K, N;
    std::cin >> K >> N;
    std::vector<int> prices(N);
    for (auto& p : prices)
        std::cin >> p;

    std::cout << maxProfit(K, prices);
}

int main(int, char**)
{
    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": ";
        io_handler();
        std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 best_time_to_buy_and_sell_stock_iv.cpp -o best_time_to_buy_and_sell_stock_iv.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address best_time_to_buy_and_sell_stock_iv.cpp -o best_time_to_buy_and_sell_stock_iv

Input

1
4 10
1 2 4 2 5 7 2 4 9 0

Output

Case #1: 15

*/


#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

// Best Time to Buy and Sell Stock with at most K transactions
// https://leetcode.com/problems/best-time-to-buy-and-sell-stock-iv

std::unordered_map<int, int> memo; // hash -> profit
std::vector<int> prices;

/**
 * @brief Unique hash of given parameters can be stored in 32 bit
 * 
 * @param k range 1..100
 * @param begin range 1..1000
 * @param end range 1..1000
 * @return int hash
 */
int hash(const int k, const int begin, const int end) {
    return (begin * 1001 + end) * 1001 + k;
}

int maxProfitInternal(const int k, const int begin, const int end) {
    const auto sz = end - begin;
    if (sz < 2)
        return 0;
    const auto h = hash(k, begin, end);
    const auto found = memo.find(h);
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
            if (k > 2)
                ans = std::max(ans, best_left[b] + maxProfitInternal(k - 1, i, end));
            else
                ans = std::max(ans, best_left[b] + max - prices[i]);
            max = std::max(max, prices[i]);
        }

    return memo[h] = ans;
}

int maxProfit(int k, std::vector<int>& _prices) {
    prices = _prices;
    return maxProfitInternal(k, 0, int(prices.size()));
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

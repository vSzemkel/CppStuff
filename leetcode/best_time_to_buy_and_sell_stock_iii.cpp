
#include <algorithm>
#include <iostream>
#include <vector>

// Best Time to Buy and Sell Stock with at most two transactions
// https://leetcode.com/problems/best-time-to-buy-and-sell-stock-iii

int bestTransaction(int* prices, int sz) {
    if (sz-- < 2)
        return 0;

    int ans{}, min{*prices++};
    for (auto p = prices; sz; --sz, ++p) {
        ans = std::max(ans, *p - min);
        min = std::min(min, *p);
    }

    return ans;
}

int maxProfit(std::vector<int>& prices) {
    const auto sz = int(prices.size());
    int ans{};
    for (int first_size = 1; first_size <= sz; ++first_size) {
        const auto first_profit = bestTransaction(prices.data(), first_size);
        const auto second_profit = bestTransaction(prices.data() + first_size, sz - first_size);
        ans  = std::max(ans, first_profit + second_profit);
    }

    return ans;
}

static void io_handler()
{
    int N;
    std::cin >> N;
    std::vector<int> prices(N);
    for (auto& p : prices)
        std::cin >> p;

    std::cout << maxProfit(prices);
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
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 best_time_to_buy_and_sell_stock_iii.cpp -o best_time_to_buy_and_sell_stock_iii.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address best_time_to_buy_and_sell_stock_iii.cpp -o n_queens-ii

Input

1
8
3 3 5 0 0 3 1 4

Output

Case #1: 6

*/

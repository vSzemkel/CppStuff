
#include <algorithm>
#include <iostream>
#include <vector>

// Best Time to Buy and Sell Stock with at most two transactions
// https://leetcode.com/problems/best-time-to-buy-and-sell-stock-iii

/*
int n = prices.size();
if(n==0)return 0;
int s1 = -prices[0], s2 = INT_MIN, s3 = INT_MIN, s4 = INT_MIN;
for(int i = 1;i<n;i++){
    s1 = max(s1,-prices[i]);
    s2 = max(s2,s1+prices[i]);
    s3 = max(s3,s2-prices[i]);
    s4 = max(s4,s3+prices[i]);
}
return max(0,s4);
*/

int maxProfit(std::vector<int>& prices) {
    const auto sz = int(prices.size());
    if (sz < 2)
        return 0;

    std::vector<int> best_left(sz + 1); // best[i] is best of i left most elements
    int ans{}, min{prices.front()};
    for (int i = 1; i < sz; ++i) {
        best_left[i + 1] = std::max(best_left[i], prices[i] - min);
        min = std::min(min, prices[i]);
    }

    ans = best_left[sz];
    int max = prices.back();
    for (int i = sz - 2; i > 1; --i) { // left of size >= 2 to be effective
        ans = std::max(ans, max - prices[i] + best_left[i]);
        max = std::max(max, prices[i]);
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
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address best_time_to_buy_and_sell_stock_iii.cpp -o best_time_to_buy_and_sell_stock_iii

Input

1
8
3 3 5 0 0 3 1 4

Output

Case #1: 6

*/

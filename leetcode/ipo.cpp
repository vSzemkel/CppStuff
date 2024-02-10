
#include <iostream>
#include <queue>
#include <vector>

// IPO
// https://leetcode.com/problems/ipo

/**
 * @brief For optimal solution we could choose less demanding project first
 * Insert all awailable projects to priority queue and choose one with highest profit
 * Continue until k projects are selected
 */
static int findMaximizedCapital(int k, int w, std::vector<int>& profits, std::vector<int>& capital)
{
    struct project_t
    {
        int capital;
        int profit;
        bool operator<(const project_t& rhs)
        {
            return capital < rhs.capital;
        }
    };

    const int N = int(profits.size());
    std::vector<project_t> projects(N);
    for (int i = 0; i < N; ++i) {
        projects[i].capital = capital[i];
        projects[i].profit = profits[i];
    }
    std::sort(projects.begin(), projects.end());

    int ans{w}, i{};
    std::priority_queue<int> pq;
    for (; k; --k) {
        while (i < N && projects[i].capital <= ans)
            pq.push(projects[i++].profit);
        if (pq.empty())
            break;
        ans += pq.top();
        pq.pop();
    }

    return ans;
}

static void io_handler()
{
    int N, K, W;
    std::cin >> N >> K >> W;
    std::vector<int> profits(N), capitals(N);
    for (auto& p : profits)
        std::cin >> p;
    for (auto& c : capitals)
        std::cin >> c;

    std::cout << findMaximizedCapital(K, W, profits, capitals);
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
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 ipo.cpp -o ipo.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address ipo.cpp -o n_queens-ii

Input

1
3 2 0
1 2 3
0 1 1

Output

4

*/

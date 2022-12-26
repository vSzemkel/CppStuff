
#include <algorithm>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>

// Food Stalls
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000051061/0000000000161476

constexpr const int64_t INF = 1e18; // big cost

/**
 * @brief Observation: if stalls positions are given, warehouse position is in the middle of them
 */
static void solve() {
    int K, N;
    std::cin >> K >> N;
    std::vector<std::pair<int,int>> location(N); // {position, cost}
    for (auto& p : location)
        std::cin >> p.first;
    for (auto& c : location)
        std::cin >> c.second;
    std::sort(location.begin(), location.end());

    int64_t ans{INF};
    const int left = K / 2;
    const int right = K - left;
    for (int w = left; w < N - right; ++w) {
        int64_t lsum{0};
        std::priority_queue<int64_t> pq;
        for (int i = 0; i < left; ++i) {
            const int c = location[w].first - location[i].first + location[i].second;
            lsum += c;
            pq.push(c);
        }
        for (int i = left; i < w; ++i) {
            const int c = location[w].first - location[i].first + location[i].second;
            pq.push(c);
            lsum += c - pq.top();
            pq.pop();
        }

        pq = std::priority_queue<int64_t>{};
        int64_t rsum{0};
        for (int i = w + 1, z = right; z; --z, ++i) {
            const int c = location[i].first - location[w].first + location[i].second;
            rsum += c;
            pq.push(c);
        }
        for (int i = w + right + 1; i < N; ++i) {
            const int c = location[i].first - location[w].first + location[i].second;
            pq.push(c);
            rsum += c - pq.top();
            pq.pop();
        }

        ans = std::min(ans, lsum + location[w].second + rsum);
    }

    std::cout << ans;
}

static void solve_naive() {
    int K, N;
    std::cin >> K >> N;
    std::vector<int> position(N), cost(N);
    for (auto& p : position)
        std::cin >> p;
    for (auto& c : cost)
        std::cin >> c;

    std::vector<int64_t> dp(N, INF); // warehouse -> cost
    std::vector<std::unordered_set<int>> taken(N); // warehouse -> stalls
    for (int p = 0; p < N; ++p) {
        dp[p] = cost[p];
        taken[p] = {p};
    }

    for (int k = 1; k <= K; ++k) {
        std::vector<int64_t> ndp(N, INF);
        std::vector<std::unordered_set<int>> ntaken(N);
        for (int w = 0; w < N; ++w)
            for (int s = 0; s < N; ++s) {
                if (taken[w].count(s) > 0)
                    continue;
                const auto can = dp[w] + std::abs(position[w] - position[s]) + cost[s];
                if (can < ndp[w]) {
                    ndp[w] = can;
                    ntaken[w] = taken[w];
                    ntaken[w].insert(s);
                }
            }

        dp = std::move(ndp);
        taken = std::move(ntaken);
    }

    std::cout << *std::min_element(dp.begin(), dp.end());
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 food_stalls.cpp -o food_stalls.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address food_stalls.cpp -o food_stalls

Run:
food_stalls.exe < food_stalls.in

Input:

3
2 4
1 2 3 10
100 70 80 20
1 5
150 300 301 400 700
8 35 26 5 2
6 7
22 21 20 23 26 25 24
10 10 10 10 10 10 10

Output:

Case #1: 178
Case #2: 62
Case #3: 82

*/

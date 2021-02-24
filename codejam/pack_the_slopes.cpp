
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

// Pack The Slopes
// https://codingcompetitions.withgoogle.com/codejam/round/000000000019ff31/00000000003b4f31

struct edge_t {
    int destination;
    int64_t capacity;
    int64_t cost;
};

// Slopes graph
std::vector<std::vector<edge_t>> g_slopes;

// Subproblem of packing the slopes starting from i-th station
// Queue contains used edges with total cost of this usage and flow. Expensive first
// Second pair element contains number of skiers starting from i-th station
// {{total cost, flow}, num_of_skies}
std::vector<std::pair<std::priority_queue<std::pair<int64_t, int64_t>>, int64_t>> g_dp;

void compute(const int slope, const int64_t baseline_cost)
{
    for (const auto& [to, cap, cost] : g_slopes[slope]) {
        const auto next_cost = baseline_cost + cost;
        auto& npq = g_dp[to].first;
        npq.push({next_cost, cap});
        compute(to, next_cost);
        auto& next_skies = g_dp[to].second;
        next_skies += cap;
        while (cap < next_skies) {
            const auto [expensive, count] = npq.top(); npq.pop();
            const auto delta = std::min(count, next_skies - cap);
            next_skies -= delta;
            if (delta < count)
                npq.push({expensive, count - delta});
        }

        g_dp[slope].second += g_dp[to].second;
        auto& cpq = g_dp[slope].first;
        if (cpq.size() < npq.size())
            std::swap(cpq, npq);

        while (!npq.empty()) {
            cpq.push(npq.top());
            npq.pop();
        }
    }
}

void solve() {
    int N; std::cin >> N;
    g_dp.assign(N, {});
    g_slopes.assign(N, {});
    for (int i = 1; i < N; ++i) {
        int from, to; int64_t skiers, cost; 
        std::cin >> from >> to >> skiers >> cost; --from, --to;
        g_slopes[from].push_back({to, skiers, cost});
    }

    compute(0, 0);
    int64_t cost{0};
    auto& pq = g_dp[0].first;
    while (!pq.empty()) {
        const auto& edge = pq.top();
        cost += edge.first * edge.second;
        pq.pop();
    }
    std::cout << g_dp[0].second << " " << cost << '\n';
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve();
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 pack_the_slopes.cpp -o pack_the_slopes.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 pack_the_slopes.cpp -o pack_the_slopes.o

Run:
pack_the_slopes.exe < pack_the_slopes.in

Input:

2
4
1 2 2 5
1 3 2 5
3 4 1 -2
7
4 7 2 2
1 3 5 5
1 4 2 -1
3 2 3 -2
3 5 2 -1
3 6 2 2

Output:


*/

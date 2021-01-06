
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <numeric>
#include <queue>
#include <utility>
#include <vector>

// Golden Stone
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ff47/00000000003bef29

using int_t = uint16_t;
using vec = std::vector<uint16_t>;

int_t g_V, g_E, g_S, g_R;
int64_t g_max_cost = pow(10, 12);
std::vector<vec> g_city2cities;
std::vector<vec> g_city2stones;
std::vector<vec> g_stone2recipes;
std::vector<vec> g_recipe_ingridients;
std::vector<int_t> g_recipe_produces;

void parse_input()
{
    int_t c1, c2;
    std::cin >> g_V >> g_E >> g_S >> g_R;

    g_city2cities.assign(g_V, {});
    for (int i = 0; i < g_E; ++i) {
        std::cin >> c1 >> c2; --c1; --c2;
        g_city2cities[c1].push_back(c2);
        g_city2cities[c2].push_back(c1);
    }

    g_city2stones.assign(g_V, {});
    for (int i = 0; i < g_V; ++i) {
        std::cin >> c1;
        for (int j = 0; j < c1; ++j) {
            std::cin >> c2; --c2;
            g_city2stones[i].push_back(c2);
        }
    }

    g_recipe_produces.resize(g_R);
    g_stone2recipes.assign(g_S, {});
    g_recipe_ingridients.assign(g_R, {});
    for (int i = 0; i < g_R; ++i) {
        std::cin >> c1;
        vec& tmp = g_recipe_ingridients[i];
        tmp.resize(c1);
        for (int j = 0; j < c1; ++j) {
            std::cin >> c2; --c2;
            g_stone2recipes[c2].push_back(i);
            tmp[j] = c2;
        }
        std::cin >> c2; --c2;
        g_recipe_produces[i] = c2;
    }
}

int64_t solve()
{
    using cost_t = std::pair<int64_t, std::pair<int_t, int_t>>; // {cost, {stone, city}}

    parse_input();
    std::priority_queue<cost_t, std::vector<cost_t>, std::greater<>> pq;
    std::vector<std::vector<int64_t>> cost(g_S, std::vector<int64_t>(g_V, g_max_cost));
    for (int_t c = 0; c < g_V; ++c)
        for (const auto s : g_city2stones[c]) {
            cost[s][c] = 0;
            pq.push({0, {s, c}});
        }

    while (!pq.empty()) {
        const auto cell = pq.top(); pq.pop();
        const int64_t cell_cost = cell.first;
        const int_t stone = cell.second.first;
        const int_t city = cell.second.second;
        // try transport stone to nearby city
        for (const auto adj : g_city2cities[city])
            if (cell_cost + 1 < cost[stone][adj]) {
                cost[stone][adj] = cell_cost + 1;
                pq.push({cell_cost + 1, {stone, adj}});
            }
        // try craft new stone
        for (const auto& r : g_stone2recipes[stone]) {
            auto& stones = g_city2stones[city];
            const auto p = g_recipe_produces[r];
            const auto& ingredients = g_recipe_ingridients[r];
            if (std::all_of(stones.begin(), stones.end(), [p](const auto s){ return s != p; })) {
                const int64_t craft_baseline_cost = std::accumulate(ingredients.begin(), ingredients.end(), 0LL, [&cost, city](const auto acc, const auto s){ return acc + cost[s][city]; });
                if (craft_baseline_cost < cost[p][city]) {
                    cost[p][city] = craft_baseline_cost;
                    pq.push({craft_baseline_cost, {p, city}});
                }
            }
        }
    }

    int64_t ret = std::numeric_limits<int64_t>::max();
    for (const auto ca : cost[0]) 
        ret = std::min(ret, ca);

    return ret >= g_max_cost ? -1 : ret;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": " << solve() << "\n";
    }
}

/*
clang++.exe -Wall -ggdb3 -O0 -std=c++17 golden_stone.cpp -o golden_stone.exe
g++ -Wall -ggdb3 -Og -std=c++17 golden_stone.cpp -o golden_stone.o
golden_stone.exe < golden_stone.in

Input:

3
4 3 4 1
1 2
1 3
1 4
0
1 2
1 3
1 4
3 2 3 4 1
4 3 4 1
1 2
1 3
1 4
0
2 2 3
1 3
1 4
3 2 3 4 1
2 1 4 2
1 2
2 2 3
1 4
3 2 3 4 1
2 2 3 4

Output:

Case #1: 3
Case #2: 2
Case #3: 0

*/

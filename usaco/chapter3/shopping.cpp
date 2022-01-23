/*
ID: marcin.8
LANG: C++
TASK: shopping
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=W8tifV1HCKr&S=shopping
*/

#include <algorithm>
#include <array>
#include <assert.h>
#include <bitset>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <filesystem>
#include <fstream>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <stdlib.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <vector>

std::ifstream task_in("shopping.in");
std::ofstream task_out("shopping.out");

using node_t = std::unordered_map<int, char>; // product -> quantity; char as product count <= 5
using offer_t = std::pair<int, node_t>;

constexpr int INF = 1e09;

/**
 * @brief Find shortest path in reqirements graph from init to {0, 0, ..}
 * 
 * This is greedy solution, as we know that special offers are better then regular
 * The rule is: apply special first, then regular ones
 */
int main(int, char**)
{
    std::vector<offer_t> offers; // items, price
    std::unordered_map<int, int> regular;
    // read promotions
    int special; task_in >> special;
    for (int i = 0; i < special; ++i) {
        node_t o{};
        int sz, c, q; task_in >> sz; // promo_items, product, quantity
        for (int z = sz; z; --z) {
            task_in >> c >> q;
            o[--c] = q;
        }
        int p; task_in >> p;
        // collapse single product single quantity promotions
        if (sz > 1 || q > 1) {
            offers.emplace_back(p, o);
        } else
            regular[c] = p;
    }
    // read regular prices and requirements
    node_t req{}, target{};
    int max_prod{0}, normal;
    task_in >> normal;
    for (int i = 0; i < normal; ++i) {
        int c, k, p; task_in >> c >> k >> p;
        max_prod = std::max(max_prod, c);
        if (regular.count(--c) == 0)
            regular[c] = p;
        req[c] = k;
        target[c] = 0;
    }
    // collapse single product multiple quantity promotions if worse then single quantity promotions
    for (auto it = offers.begin(); it != offers.end(); ++it) {
        auto& [p, o] = *it;
        if (o.size() == 1) {
            const int c = o.begin()->first;
            if (regular.count(c)) {
                const auto s = double(p) / o[c];
                if (s > regular[c])
                    it = offers.erase(it);
            }
        }
    }

    int ans = INF;
    const auto comp = [](const offer_t& n1, const offer_t& n2){ return n1.first > n2.first; };
    std::priority_queue<offer_t, std::vector<offer_t>, decltype(comp)> qq{comp};
    qq.push({0, req}); // {req, price so far}
    while (!qq.empty()) {
        auto [price, req] = qq.top(); qq.pop();
        if (req == target) {
            ans = std::min(ans, price);
            continue;
        }
        bool special_used{false};
        for (const auto& [p, o] : offers) {
            int i = 0;
            node_t next{req};
            for (; i < max_prod; ++i) {
                const auto it = o.find(i);
                if (it != o.end()) {
                    next[i] -= it->second;
                    if (next[i] < 0)
                        break;
                }
            }
            if (i == max_prod) {
                special_used = true;
                qq.push({price + p, next});
            }
        }

        if (!special_used) {
            int reg_price{0};
            for (int i = 0; i < max_prod; ++i)
                if (req[i] > 0)
                    reg_price += regular[i] * req[i];
            qq.push({price + reg_price, target});
        }
    }

    task_out << ans << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 shopping.cpp -o shopping.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address shopping.cpp -o shopping

Input:


Output:


*/

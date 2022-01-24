/*
ID: marcin.8
LANG: C++
TASK: shopping
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=W8tifV1HCKr&S=shopping
*/

#include <fstream>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

std::ifstream task_in("shopping.in");
std::ofstream task_out("shopping.out");

using node_t = std::unordered_map<int, char>; // product -> quantity; char as product count <= 5
using offer_t = std::pair<int, node_t>;

struct node_hasher_t {
    size_t operator()(const node_t& n) const {
        int ret{0};
        for (const auto& g : n)
            ret = ret * 6 + g.second;
        return ret;
    }
};

constexpr int INF = 1e09;
constexpr int MAX = 6 * 6 * 6 * 6 * 6; // Number of possible requirement combinations for 5 products and max 5 quantities

/**
 * @brief Find shortest path in reqirements graph from init to {0, 0, ..}
 * 
 * This is greedy solution, it can be sorted out as [special, .., regular]
 * Exclude promotions that are worse then regular purcheases
 * Exclude promotions that includes not needed products
 * The rule is: apply special first, then regular ones
 */
int main(int, char**)
{
    std::vector<offer_t> offers; // items, price
    std::unordered_map<int, int> regular; // product, price
    // read promotions
    int special; task_in >> special;
    for (int i = special; i; --i) {
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
    node_t requirement{}, target{};
    int normal; task_in >> normal;
    for (int z = normal; z; --z) {
        int c, k, p; task_in >> c >> k >> p;
        if (regular.count(--c) == 0)
            regular[c] = p;
        requirement[c] = k;
        target[c] = 0;
    }
    // ignore con promotions
    for (auto it = offers.begin(); it != offers.end(); ) {
        int reg_sum{0};
        bool extra_product{false};
        for (const auto& g : it->second) {
            reg_sum += g.second * regular[g.first];
            /*if (requirement.find(g.first) == requirement.end()) THIS EFFECTIVELY SLOWS DOWN
                extra_product = true;*/
        }
        if (reg_sum < it->first || extra_product)
            it = offers.erase(it);
        else
            ++it;
    }

    node_hasher_t nh;
    std::vector<bool> in_queue(MAX);
    std::vector<int> best(MAX, INF); // best price for going this far
    std::queue<node_t> qq;
    qq.push(requirement);
    best[nh(requirement)] = 0;
    while (!qq.empty()) {
        const auto req = qq.front(); qq.pop();
        const auto hash = nh(req);
        in_queue[hash] = false;
        if (req == target)
            continue;

        bool special_used{false};
        const auto price = best[hash]; // price for reaching req
        for (const auto& [p, o] : offers) {
            bool ok{true};
            node_t next{req};
            for (const auto& g : o) {
                next[g.first] -= g.second;
                if (next[g.first] < 0) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                special_used = true;
                const auto next_h = nh(next);
                const int can = price + p;
                if (can < best[next_h])
                    best[next_h] = can;
                if (!in_queue[next_h]) {
                    in_queue[next_h] = true;
                    qq.push(next);
                }
            }
        }

        if (!special_used) {
            int reg_price{0};
            for (const auto& g : req)
                reg_price += g.second * regular[g.first];
            if (price + reg_price < best[0])
                best[0] = price + reg_price;
        }
    }

    task_out << best[0] << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 shopping.cpp -o shopping.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address shopping.cpp -o shopping

Input:

95
1 25 1 183
1 25 2 354
1 25 3 549
1 25 4 748
1 25 5 915
1 94 1 126
2 94 1 25 1 318
2 94 1 25 2 463
2 94 1 25 3 662
1 94 2 276
2 94 2 25 1 429
2 94 2 25 2 602
1 94 3 401
2 94 3 25 1 557
1 94 4 512
1 94 5 625
1 63 1 99
2 63 1 25 1 284
2 63 1 25 2 462
2 63 1 25 3 650
2 63 1 94 1 218
3 63 1 94 1 25 1 424
2 63 1 94 2 343
2 63 1 94 3 490
1 63 2 207
2 63 2 25 1 385
2 63 2 25 2 575
2 63 2 94 1 327
2 63 2 94 2 478
1 63 3 284
2 63 3 25 1 466
2 63 3 94 1 448
1 63 4 405
1 63 5 517
1 72 1 159
2 72 1 25 1 342
2 72 1 25 2 496
2 72 1 25 3 704
2 72 1 94 1 286
3 72 1 94 1 25 1 489
2 72 1 94 2 412
2 72 1 94 3 570
2 72 1 63 1 277
3 72 1 63 1 25 1 420
3 72 1 63 1 94 1 411
2 72 1 63 2 348
2 72 1 63 3 438
1 72 2 315
2 72 2 25 1 518
2 72 2 25 2 660
2 72 2 94 1 450
2 72 2 94 2 618
2 72 2 63 1 432
2 72 2 63 2 537
1 72 3 472
2 72 3 25 1 686
2 72 3 94 1 632
2 72 3 63 1 605
1 72 4 688
1 72 5 805
1 51 1 190
2 51 1 25 1 373
2 51 1 25 2 540
2 51 1 25 3 727
2 51 1 94 1 304
3 51 1 94 1 25 1 477
2 51 1 94 2 446
2 51 1 94 3 547
2 51 1 63 1 278
3 51 1 63 1 25 1 460
3 51 1 63 1 94 1 404
2 51 1 63 2 392
2 51 1 63 3 473
2 51 1 72 1 337
3 51 1 72 1 25 1 551
3 51 1 72 1 94 1 502
3 51 1 72 1 63 1 467
2 51 1 72 2 526
2 51 1 72 3 643
1 51 2 380
2 51 2 25 1 525
2 51 2 25 2 754
2 51 2 94 1 490
2 51 2 94 2 656
2 51 2 63 1 463
2 51 2 63 2 549
2 51 2 72 1 512
2 51 2 72 2 666
1 51 3 570
2 51 3 25 1 700
2 51 3 94 1 663
2 51 3 63 1 652
2 51 3 72 1 728
1 51 4 711
1 51 5 889
5
51 5 202
72 5 185
63 5 110
94 5 147
25 5 199

Output:

3602

*/

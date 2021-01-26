
#include <assert.h>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

// Emacs++
// https://codingcompetitions.withgoogle.com/codejam/round/000000000019ffb9/000000000033893b

const auto big64 = std::numeric_limits<int64_t>::max();

enum class move_t : uint8_t {
    left,
    right,
    transfer,
    any
};

std::vector<int> parentheses_map(const std::string& ps)
{
    const auto size = (int)ps.size();
    std::vector<int> stage, ret(size);
    stage.reserve(size / 2);
    for (int i = 0; i < size; ++i) {
        if (ps[i] == '(')
            stage.push_back(i);
        else {
            const int opening = stage.back();
            ret[opening] = i - opening;
            ret[i] = opening - i;
            stage.pop_back();
        }
    }
    return ret;
}

uint64_t solve() {
    int K, Q; std::cin >> K >> Q;
    std::string code; std::cin >> code;

    const std::vector<int> transfer = parentheses_map(code);

    std::map<std::pair<int, int>, uint64_t> cost;
    uint64_t c; std::cin >> c; // ignore left of 0
    for (int i = 1; i < K; ++i) {
        std::cin >> c;
        cost[{i, i - 1}] = c;
    }
    for (int i = 0; i < K - 1; ++i) {
        std::cin >> c;
        cost[{i, i + 1}] = c;
    }
    std::cin >> c; // ignore right of (K-1)
    for (int i = 0; i < K; ++i) {
        std::cin >> c;
        const int tr = transfer[i];
        if ((tr == 1 || tr == -1) && c >= cost[{i, i + tr}]) 
            continue;
        cost[{i, i + tr}] = c;
    }

    uint64_t ret{0};
    std::vector<int> S(Q);
    for (int i = 0; i < Q; ++i)
        std::cin >> S[i];
    for (int i = 0; i < Q; ++i) {
        uint64_t ca{0};
        int end; std::cin >> end; --end;
        const int start = S[i] - 1;
        // compute first valid solution for further improvement
        const int step = end - start > 0 ? 1 : -1;
        for (int j = start; j != end; j += step)
            ca += cost[{j, j + step}];
        // should never move outside of the closest to [S, E] pair of brackets
        int lower_bound{start}, upper_bound{end};
        if (lower_bound > upper_bound)
            std::swap(lower_bound, upper_bound);
        while (lower_bound >= 0) {
            if (code[lower_bound] == '(' && lower_bound + transfer[lower_bound] >= end) {
                upper_bound = lower_bound + transfer[lower_bound];
                break;
            }
            --lower_bound;
        }
        if (lower_bound < 0) {
            lower_bound = 0;
            upper_bound = K - 1;
        }
        // try to find better with DFS
        std::vector<std::tuple<int, uint64_t, move_t>> dfs;
        dfs.emplace_back(start, 0, move_t::any);
        while (!dfs.empty()) {
            const int pos = std::get<0>(dfs.back());
            uint64_t path_cost = std::get<1>(dfs.back());
            move_t last = std::get<2>(dfs.back());
            bool transfered = path_cost < 0;
            if (transfered) path_cost *= -1;
            dfs.pop_back();
            if (pos == end && path_cost < ca) {
                ca = path_cost;
                continue;
            }
            if (pos > lower_bound && last != move_t::right) {
                const auto new_cost = path_cost + cost[{pos, pos - 1}];
                if (new_cost < ca) dfs.emplace_back(pos - 1, new_cost, move_t::left);
            }
            if (pos < upper_bound && last != move_t::left) {
                const auto new_cost = path_cost + cost[{pos, pos + 1}];
                if (new_cost < ca) dfs.emplace_back(pos + 1, new_cost, move_t::right);
            }
            if (last != move_t::transfer) {
                const auto new_cost = path_cost + cost[{pos, pos + transfer[pos]}];
                if (new_cost < ca) dfs.emplace_back(pos + transfer[pos], new_cost, move_t::transfer);
            }
        }
        ret += ca;
    }

    return ret;
}

uint64_t solve2() {
    int K, Q; std::cin >> K >> Q;
    std::string code; std::cin >> code;

    // build matching parenthesis map
    const std::vector<int> transfer = parentheses_map(code);

    // this solution is good for not too much K
    std::vector<std::vector<uint64_t>> cost(K, std::vector<uint64_t>(K, big64));
    uint64_t c; std::cin >> c; // ignore left of 0
    for (int i = 1; i < K; ++i) {
        std::cin >> c;
        cost[i][i - 1] = c;
    }
    for (int i = 0; i < K - 1; ++i) {
        std::cin >> c;
        cost[i][i + 1] = c;
    }
    std::cin >> c; // ignore right of (K-1)
    for (int i = 0; i < K; ++i) {
        std::cin >> c;
        const int tr = transfer[i];
        cost[i][i] = 0;
        if ((tr == 1 || tr == -1) && c >= cost[i][i + tr]) 
            continue;
        cost[i][i + tr] = c;
    }

    for (int d = 2; d < K - 1; ++d)
        for (int i = 0; i + d < K; ++i) {
            const auto nojumpR = std::min(cost[i][i + d - 1] + cost[i + d - 1][i + d], cost[i][i + 1] + cost[i + 1][i + d]);
            cost[i][i + d] = std::min(cost[i][i + d], nojumpR);
            const auto nojumpL = std::min(cost[i + d][i + d - 1] + cost[i + d - 1][i], cost[i + d][i + 1] + cost[i + 1][i]);
            cost[i + d][i] = std::min(cost[i + d][i], nojumpL);
        }

    uint64_t ret{0};
    std::vector<int> S(Q);
    for (int i = 0; i < Q; ++i)
        std::cin >> S[i];
    for (int i = 0; i < Q; ++i) {
        int E; std::cin >> E;
        ret += cost[S[i] - 1][E - 1];
    }

    return ret;
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

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 emacs++.cpp -o emacs++.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 emacs++.cpp -o emacs++.o

Run:
emacs++.exe < emacs++.in

Input:

1
12 5
(()(((()))))
1 1 1 1 1 1 1 1 1 1 1 1
1 1 1 1 1 1 1 1 1 1 1 1
1 1 1 1 1 1 1 1 1 1 1 1
7 4 4 12 5
12 11 10 1 6

Output:

Case #1: 10

*/


#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <filesystem>
#include <format>
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
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <vector>

// Least Common Ancestor
// https://www.facebook.com/codingcompetitions/hacker-cup/2024/round-3/problems/B

struct sort_predicate {
    bool operator()(const std::pair<const int, int>& s1, const std::pair<const int, int>& s2) const {
        return s1.second < s2.second || (s1.second == s2.second && s1.first < s2.first);
    }
};

using stats_t = std::map<int, int, sort_predicate>; // {name_index, count}

struct node_t {
    int _name; // index of a name in sorted list
    int _parent;
    std::vector<int> _childs;
    stats_t _ancestors;
    stats_t _descendants;
};

int least_common(const stats_t& stats) {
    if (stats.empty())
        return 0;

    return 1 + std::min_element(stats.begin(), stats.end(),
        [](const auto& s1, const auto& s2){ return s1.second < s2.second; })->first;
}

stats_t dfs(int node_idx, const stats_t& root_path) {
    return {};
}

static int64_t solve() {
    int N, root;
    std::cin >> N;
    std::vector<std::pair<std::string, int>> names;
    std::vector<node_t> tree(N);
    for (int i = 0; i < N; ++i) {
        int p;
        std::string n;
        std::cin >> p >> n;
        if (0 < p) {
            --p;
            tree[p]._childs.push_back(i);
        } else
            root = i;
        tree[i]._parent = p;
        names.emplace_back(n, i);
    }

    std::sort(names.begin(), names.end());
    int ord{};
    std::string prev = names.front().first;
    for (const auto& [name, node_idx] : names)
        if (name == prev)
            tree[node_idx]._name = ord;
        else {
            tree[node_idx]._name = ++ord;
            prev = name;
        }

    tree[root]._descendants = dfs(root, {});

    int64_t hash{0};
    return hash;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << std::format("Case #{}: {}\n", g, solve());
    }
}

/*

Format:
clang-format -i least_common_ancestor.cpp

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 least_common_ancestor.cpp
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address least_common_ancestor.cpp
clang -Wall -Wextra -Og -ggdb3 -lstdc++ -std=c++20 least_common_ancestor.cpp

Run:
py.exe interactive_runner.py py.exe least_common_ancestor_testing_tool.py 1 -- a.exe
a.exe < least_common_ancestor.in

Input:


Output:


*/


#include <algorithm>
#include <iostream>
#include <format>
#include <string>
#include <unordered_map>
#include <vector>

// Least Common Ancestor
// https://www.facebook.com/codingcompetitions/hacker-cup/2024/round-3/problems/B

constexpr auto sort_predicate = [](const std::pair<const int, int>& s1, const std::pair<const int, int>& s2){
    return s1.second < s2.second || (s1.second == s2.second && s1.first < s2.first);
};

using stats_t = std::unordered_map<int, int>; // {name_index, count}

struct node_t {
    int _name; // index of a name in sorted list
    int _parent;
    std::vector<int> _childs;
    stats_t _ancestors;
    stats_t _descendants;
};

int U;
const int M = 998'244'353;
std::vector<node_t> tree;

int least_common(const stats_t& stats) {
    if (stats.empty())
        return 0;

    return 1 + std::min_element(stats.begin(), stats.end(), sort_predicate)->first;
}

int64_t hash() {
    int64_t ret{};
    for (const auto& node : tree) {
        ret = (ret * U + least_common(node._ancestors)) % M;
        ret = (ret * U + least_common(node._descendants)) % M;
    }

    return ret;
}

stats_t dfs(int node_idx) {
    auto& node = tree[node_idx];
    if (0 <= node._parent) {
        const auto& parent = tree[node._parent];
        node._ancestors = parent._ancestors;
        ++node._ancestors[parent._name];
    }

    stats_t ret;
    for (const int c : node._childs) {
        const auto child_descendants = dfs(c);
        for (const auto& [name, size] : child_descendants)
            ret[name] += size;
    }

    node._descendants = ret;
    ++ret[node._name];
    return ret;
}

static int64_t solve() {
    int N, root;
    std::cin >> N;
    std::vector<std::pair<std::string, int>> names;
    tree.resize(N);
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
    U = 0;
    std::string prev = names.front().first;
    for (const auto& [name, node_idx] : names)
        if (name == prev)
            tree[node_idx]._name = U;
        else {
            tree[node_idx]._name = ++U;
            prev = name;
        }
    U += 2;

    dfs(root);

    return hash();
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

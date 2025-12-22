
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
    int _least_common_ancestor;
    int _least_common_descendant;
    std::vector<int> _childs;
};

int U;
const int M = 998'244'353;
std::vector<node_t> tree;
stats_t root_path;

int least_common(const stats_t& stats) {
    if (stats.empty())
        return 0;

    return 1 + std::min_element(stats.begin(), stats.end(), sort_predicate)->first;
}

int64_t hash() {
    int64_t ret{};
    for (const auto& node : tree) {
        ret = (ret * U + node._least_common_ancestor) % M;
        ret = (ret * U + node._least_common_descendant) % M;
    }

    return ret;
}

stats_t dfs(int node_idx) {
    auto& node = tree[node_idx];
    node._least_common_ancestor = least_common(root_path);
    ++root_path[node._name];

    stats_t ret;
    for (const int c : node._childs) {
        const auto child_descendants = dfs(c);
        for (const auto& [name, size] : child_descendants)
            ret[name] += size;
    }

    if (--root_path[node._name] == 0)
        root_path.erase(node._name);
    node._least_common_descendant = least_common(ret);
    ++ret[node._name];
    return ret;
}

void dfs_loop(int node_idx) {
    struct frame_t {
        int node_id;
        size_t next_child;
        stats_t stats;
    };
    stats_t empty;
    empty.reserve(U);
    std::vector<frame_t> stack(1, {node_idx, 0, empty});
    stack.reserve(1024);
    while (!stack.empty()) {
        auto& cur = stack.back();
        auto& node = tree[cur.node_id];

        if (cur.next_child == 0)
            ++root_path[node._name];
        if (cur.next_child < node._childs.size()) {
            stack.push_back({node._childs[cur.next_child++], 0, empty});
            continue;
        }
        if (--root_path[node._name] == 0)
            root_path.erase(node._name);

        node._least_common_ancestor = least_common(root_path);
        node._least_common_descendant = least_common(cur.stats);

        if (0 <= node._parent) {
            size_t pid = stack.size() - 1;
            while (stack[pid].node_id != node._parent)
                --pid;
            auto& parent_stats = stack[pid].stats;
            if (parent_stats.size() < cur.stats.size())
                std::swap(parent_stats, cur.stats);
            for (const auto& [name, size] : cur.stats)
                parent_stats[name] += size;
            ++parent_stats[node._name];
        }
        stack.pop_back();
    }
}

static int64_t solve() {
    int N, root;
    std::cin >> N;
    std::vector<std::pair<std::string, int>> names;
    tree.assign(N, {});
    root_path.clear();
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

    dfs_loop(root);

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

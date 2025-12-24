
#include <algorithm>
#include <iostream>
#include <format>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

// Least Common Ancestor
// https://www.facebook.com/codingcompetitions/hacker-cup/2024/round-3/problems/B

static constinit const int M = 998'244'353;

struct stats_t {
    int update(int name, int delta) {
        const auto found = _stats.find(name);
        if (found != _stats.end()) {
            int base = found->second;
            _ord.erase({base, name});
            _ord.emplace(base + delta, name);
            return found->second += delta;
        }

        _ord.emplace(delta, name);
        return _stats[name] = delta;
    }

    int least_common() const {
        if (_ord.empty())
            return 0;
        return 1 + _ord.begin()->second;
    };

    size_t size() const { return _stats.size(); }

    void erase(int name) {
        const auto found = _stats.find(name);
        if (found != _stats.end()) {
            _ord.erase({found->second, name});
            _stats.erase(found);
        }
    };

    void clear() {
        _ord.clear();
        _stats.clear();
    }

    std::unordered_map<int, int> _stats; // {name_index, count}

  private:
    std::set<std::pair<int, int>> _ord; // {count, name_index}
};

struct node_t {
    int _name; // index of a name in sorted list
    int _parent;
    int _least_common_ancestor;
    int _least_common_descendant;
    std::vector<int> _childs;
};

int U;
std::vector<node_t> tree;
stats_t root_path;

stats_t dfs(int node_idx) { // stack overflow on Windows
    auto& node = tree[node_idx];
    node._least_common_ancestor = root_path.least_common();
    root_path.update(node._name, 1);

    stats_t ret;
    for (const int c : node._childs) {
        auto child_descendants = dfs(c);
        if (ret.size() < child_descendants.size())
            std::swap(ret, child_descendants);
        for (const auto& [name, size] : child_descendants._stats)
            ret.update(name, size);
    }

    if (root_path.update(node._name, -1) == 0)
        root_path.erase(node._name);

    node._least_common_descendant = ret.least_common();
    ret.update(node._name, 1);
    return ret;
}

void dfs_loop(int node_idx) {
    struct frame_t {
        int node_id;
        size_t next_child;
        stats_t stats;
    };
    std::vector<frame_t> stack(1, {node_idx, 0, stats_t{}});
    stack.reserve(U);
    while (!stack.empty()) {
        auto& cur = stack.back();
        auto& node = tree[cur.node_id];

        if (cur.next_child == 0)
            root_path.update(node._name, 1);
        if (cur.next_child < node._childs.size()) {
            stack.push_back({node._childs[cur.next_child++], 0, {}});
            continue;
        }
        if (root_path.update(node._name, -1) == 0)
            root_path.erase(node._name);

        node._least_common_ancestor = root_path.least_common();
        node._least_common_descendant = cur.stats.least_common();

        if (0 <= node._parent) {
            auto& parent_stats = stack[stack.size() - 2].stats;
            if (parent_stats.size() < cur.stats.size())
                std::swap(parent_stats, cur.stats);
            for (const auto& [name, size] : cur.stats._stats)
                parent_stats.update(name, size);
            parent_stats.update(node._name, 1);
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

    int64_t hash{};
    for (const auto& node : tree) {
        hash = (hash * U + node._least_common_ancestor) % M;
        hash = (hash * U + node._least_common_descendant) % M;
    }

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

3
2
-1 alice
1 bob
8
-1 alice
1 alice
2 bob
3 carl
3 anna
1 bob
6 alice
7 anna
10
-1 alice
1 bob
1 emily
2 alice
3 alice
3 abby
4 carl
5 abby
7 dan
7 emily

Output:

Case #1: 21
Case #2: 243150762
Case #3: 255661373

*/

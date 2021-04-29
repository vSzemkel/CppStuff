
#include <algorithm>
#include <array>
#include <assert.h>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

// Described tree - datastructure implementing elementary tree operations
// Inspired by work of neal_wu

template<typename T, bool maximum_mode = false> // inspired by neal_wu
struct rmq_t {
    rmq_t(const std::vector<T>& v = {}) : _size((int)v.size()) {
        if (_size > 0) {
            _values = v;
            build();
        }
    }

    static int highest_bit(const int x) {
        return x == 0 ? -1 : 31 - __builtin_clz(x);
    }

    // Note: breaks ties by choosing the largest index.
    int query_index(const int a, const int b) const {
        assert(0 <= a && a < b && b <= _size);
        const int level = highest_bit(b - a);
        const auto& row = _range_low[level];
        return better_index(row[a], row[b - (1 << level)]);
    }

    T query_value(const int a, const int b) const {
        return _values[query_index(a, b)];
    }

    void build(const std::vector<T>& values = {}) {
        if (!values.empty()) {
            _size = (int)values.size();
            _values = values;
        }

        const int levels = highest_bit(_size) + 1;

        _range_low.resize(levels);
        for (int k = 0; k < levels; ++k)
            _range_low[k].resize(_size - (1 << k) + 1);

        auto& prev = _range_low[0];
        std::iota(prev.begin(), prev.end(), 0);
        for (int k = 1; k < levels; ++k) {
            auto& curr = _range_low[k];
            for (int i = 0; i <= _size - (1 << k); ++i)
                curr[i] = better_index(prev[i], prev[i + (1 << (k - 1))]);
            prev = curr;
        }
    }

  private:
    // Note: when `values[a] == values[b]`, returns b.
    int better_index(const int a, const int b) const {
        return (maximum_mode ? _values[b] < _values[a] : _values[a] < _values[b]) ? a : b;
    }

    int _size{0};
    std::vector<T> _values;
    std::vector<std::vector<int>> _range_low;
};

struct desctree_t {
    desctree_t(const int n = 0) : _size(n) {
        init(n);
        _adj.resize(n);
    }

    // Warning: this does not call build().
    desctree_t(const std::vector<std::vector<int>>& adj) : _size((int)adj.size()) {
        init(_size);
        _adj = adj;
    }

    void init(const int n) {
        _parent.assign(n, -1);
        _subtree_size.assign(n, 1);
        _depth.resize(n);
        _euler_first_rank.resize(n);
        _dfs_rank.resize(n);
        _dfs_rank_end.resize(n);
        _postorder.resize(n);
        _dfs_list.resize(n);
        _heavy_root.resize(n);
        _subtree_paths.resize(n);
        _paths.resize(n);
    }

    void add_edge(const int a, const int b) {
        _adj[a].push_back(b);
        _adj[b].push_back(a);
    }

    int degree(const int v) const {
        return (int)_adj[v].size() + (_built && _parent[v] >= 0);
    }

    void dfs(const int node, const int par) {
        _parent[node] = par;
        _depth[node] = par < 0 ? 0 : _depth[par] + 1;

        // Erase the edge to parent.
        auto& ngb = _adj[node];
        ngb.erase(std::remove(ngb.begin(), ngb.end(), par), ngb.end());

        for (const int child : ngb) {
            dfs(child, node);
            _subtree_size[node] += _subtree_size[child];
            _paths[node] += _paths[child] + _subtree_size[child];
            _subtree_paths[node] += _subtree_paths[child];
        }

        _subtree_paths[node] += _paths[node];

        // Heavy-light subtree reordering.
        std::sort(ngb.begin(), ngb.end(), [&](const int a, const int b) {
            return _subtree_size[a] > _subtree_size[b];
        });
    }

    void tour_dfs(const int node, const bool heavy) {
        _heavy_root[node] = heavy ? _heavy_root[_parent[node]] : node;
        _euler_first_rank[node] = (int)_euler.size();
        _euler.push_back(node);
        _dfs_list[_tour] = node;
        _dfs_rank[node] = _tour++;

        bool heavy_child{true};
        for (const int child : _adj[node]) {
            tour_dfs(child, heavy_child);
            _euler.push_back(node);
            heavy_child = false;
        }

        _dfs_rank_end[node] = _tour;
        _postorder[node] = _post_tour++;
    }

    void build(const int root = -1, const bool build_rmq = true) {
        if (0 <= root && root < _size)
            dfs(root, -1);

        // for disconnected regions
        for (int i = 0; i < _size; ++i)
            if (i != root && _parent[i] < 0)
                dfs(i, -1);

        _euler.clear();
        _euler.reserve(2 * _size);
        _tour = _post_tour = 0;

        for (int i = 0; i < _size; ++i)
            if (_parent[i] < 0) {
                tour_dfs(i, false);
                // Add a -1 in between connected components to help us detect when nodes aren't connected.
                _euler.push_back(-1);
            }

        _rev_dfs_list = _dfs_list;
        std::reverse(_rev_dfs_list.begin(), _rev_dfs_list.end());

        if (build_rmq) {
            assert((int)_euler.size() == 2 * _size);
            std::vector<int> euler_depths;
            euler_depths.reserve(_euler.size());
            for (const int node : _euler)
                euler_depths.push_back(node < 0 ? node : _depth[node]);
            _rmq.build(euler_depths);
        }

        _built = true;
    }

    std::pair<int, std::array<int, 2>> get_diameter() const {
        assert(_built);

        // We find the maximum of depth[u] - 2 * depth[x] + depth[v] where u, x, v occur in order in the Euler tour.
        std::pair<int, int> u_max = {-1, -1};
        std::pair<int, int> ux_max = {-1, -1};
        std::pair<int, std::array<int, 2>> uxv_max = {-1, {-1, -1}};

        for (const int node : _euler) {
            if (node < 0) break;
            u_max = std::max(u_max, {_depth[node], node});
            ux_max = std::max(ux_max, {u_max.first - 2 * _depth[node], u_max.second});
            uxv_max = std::max(uxv_max, {ux_max.first + _depth[node], {ux_max.second, node}});
        }

        return uxv_max;
    }

    // Note: returns -1 if `a` and `b` aren't connected.
    int get_lca(int a, int b) const {
        a = _euler_first_rank[a];
        b = _euler_first_rank[b];

        if (a > b)
            std::swap(a, b);

        return _euler[_rmq.query_index(a, b + 1)];
    }

    bool is_ancestor(const int a, const int b) const {
        return _dfs_rank[a] <= _dfs_rank[b] && _dfs_rank[b] < _dfs_rank_end[a];
    }

    bool on_path(const int x, const int a, const int b) const {
        return (is_ancestor(x, a) || is_ancestor(x, b)) && is_ancestor(get_lca(a, b), x);
    }

    int get_dist(const int a, const int b) const {
        return _depth[a] + _depth[b] - 2 * _depth[get_lca(a, b)];
    }

    int get_paths_len_to(const int node) const {
        const int parent = _parent[node];
        if (parent == -1)
            return _paths[0];
        return get_paths_len_to(parent) + _subtree_size[0] - 2 * _subtree_size[node];
    }

    // Returns the child of `a` that is an ancestor of `b`. Assumes `a` is a strict ancestor of `b`.
    int child_ancestor(int a, int b) const {
        assert(a != b);
        assert(is_ancestor(a, b));

        // Note: this depends on rmq_t breaking ties by latest index.
        const int child = _euler[_rmq.query_index(_euler_first_rank[a], _euler_first_rank[b] + 1) + 1];
        assert(_parent[child] == a);
        assert(is_ancestor(child, b));
        return child;
    }

    int get_kth_ancestor(int a, int k) const {
        while (a >= 0) {
            int root = _heavy_root[a];

            if (_depth[root] <= _depth[a] - k)
                return _dfs_list[_dfs_rank[a] - k];

            k -= _depth[a] - _depth[root] + 1;
            a = _parent[root];
        }

        return a;
    }

    int get_kth_node_on_path(int a, int b, int k) const {
        const int anc = get_lca(a, b);
        const int first_half = _depth[a] - _depth[anc];
        const int second_half = _depth[b] - _depth[anc];
        assert(0 <= k && k <= first_half + second_half);

        if (k < first_half)
            return get_kth_ancestor(a, k);
        else
            return get_kth_ancestor(b, first_half + second_half - k);
    }

    // Note: this is the LCA of any two nodes out of three when the third node is the root.
    // It is also the node with the minimum sum of distances to all three nodes (the centroid of the three nodes).
    int get_common_node(const int a, const int b, const int c) const {
        // Return the deepest node among lca(a, b), lca(b, c), and lca(c, a).
        int x = get_lca(a, b);
        const int y = get_lca(b, c);
        const int z = get_lca(c, a);
        x = _depth[y] > _depth[x] ? y : x;
        x = _depth[z] > _depth[x] ? z : x;
        return x;
    }

    // Given a subset of k tree nodes, computes the minimal subtree that contains all the nodes (at most 2k - 1 nodes).
    // Returns a list of {node, parent} for every node in the subtree. Runs in O(k log k).
    std::vector<std::pair<int, int>> compress_tree(std::vector<int> nodes) const {
        if (nodes.empty())
            return {};

        auto&& compare_tour = [&](const int a, const int b) { return _dfs_rank[a] < _dfs_rank[b]; };
        std::sort(nodes.begin(), nodes.end(), compare_tour);

        int k = (int)nodes.size();
        for (int i = 0; i < k - 1; ++i)
            nodes.push_back(get_lca(nodes[i], nodes[i + 1]));

        std::sort(nodes.begin() + k, nodes.end(), compare_tour);
        std::inplace_merge(nodes.begin(), nodes.begin() + k, nodes.end(), compare_tour);
        nodes.erase(std::unique(nodes.begin(), nodes.end()), nodes.end());
        std::vector<std::pair<int, int>> result = {{nodes[0], -1}};

        for (int i = 1; i < (int)nodes.size(); ++i)
            result.emplace_back(nodes[i], get_lca(nodes[i], nodes[i - 1]));

        return result;
    }

  // private:
    int _size{};
    int _tour, _post_tour;
    std::vector<std::vector<int>> _adj;
    std::vector<int> _parent, _depth, _subtree_size;
    std::vector<int> _euler, _euler_first_rank;
    std::vector<int> _dfs_rank, _dfs_rank_end, _postorder;
    std::vector<int> _dfs_list, _rev_dfs_list;
    std::vector<int> _paths, _subtree_paths;
    std::vector<int> _heavy_root;
    rmq_t<int> _rmq;
    bool _built{};
};

constexpr int N = 10;
desctree_t g_tree(N);

static void init()
{
    g_tree.add_edge(0, 1);
    g_tree.add_edge(2, 0);
    g_tree.add_edge(1, 3);
    g_tree.add_edge(3, 6);
    g_tree.add_edge(7, 3);
    g_tree.add_edge(7, 9);
    g_tree.add_edge(4, 2);
    g_tree.add_edge(2, 5);
    g_tree.add_edge(5, 8);
}

static void test()
{
    std::vector<int> subtree_of_1;
    for (int i = g_tree._dfs_rank[1]; i < g_tree._dfs_rank_end[1]; ++i)
        subtree_of_1.push_back(g_tree._dfs_list[i]);
    assert((subtree_of_1 == std::vector<int>{1, 3, 7, 9, 6}));

    for (int i = 0; i < N; ++i)
        assert(g_tree._subtree_size[i] == g_tree._dfs_rank_end[i] - g_tree._dfs_rank[i]);

    assert(g_tree.on_path(3, 9, 0));
    assert(!g_tree.on_path(4, 0, 9));

    const auto ct = g_tree.compress_tree({6, 9});
    assert(ct.size() == 3);

    const auto diameter = g_tree.get_diameter();
    std::cout << "Nodes count: " << g_tree._subtree_size[0] << '\n';
    std::cout << "Diameter: " << diameter.first << " (" << diameter.second[0] << ", " << diameter.second[1] << ")\n";
    std::cout << "Root paths sum: " << g_tree._paths[0] << '\n';
    std::cout << "Subtrees paths sum: " << g_tree._subtree_paths[0] << '\n';
    std::cout << "Degree of node 3: " << g_tree.degree(2) << '\n';
    std::cout << "Sum of paths to 3: " << g_tree.get_paths_len_to(2) << '\n';
    std::cout << "Child ancestor of 0 on path to 8: " << g_tree.child_ancestor(0, 8) << '\n';
    std::cout << "3rd node on path from 1 to 8: " << g_tree.get_kth_node_on_path(1, 8, 3) << '\n';
    std::cout << "2nd ancestor of 7: " << g_tree.get_kth_ancestor(7, 2) << '\n';
    std::cout << "Common node of 4, 5, 8: " << g_tree.get_common_node(4, 5, 8) << '\n';
    std::cout << "Lowest common ancestor of 6 and 9: " << g_tree.get_lca(6, 9) << '\n';
    std::cout << "Distance between 5 and 7: " << g_tree.get_dist(5, 7) << '\n';
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    init();
    g_tree.build();
    test();
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 desctree.cpp -o desctree.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address desctree.cpp -o desctree.o

Output:

Nodes count: 10
Diameter: 7 (9, 8)
Root paths sum: 21
Subtrees paths sum: 39
Degree of node 3: 3
Sum of paths to 3: 23
Child ancestor of 0 on path to 8: 2
3rd node on path from 1 to 8: 5
2nd ancestor of 7: 1
Common node of 4, 5, 8: 5
Lowest common ancestor of 6 and 9: 3
Distance between 5 and 7: 5

*/

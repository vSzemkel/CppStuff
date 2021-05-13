
#include <algorithm>
#include <array>
#include <assert.h>
#include <functional>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

// Described tree - datastructure implementing elementary tree operations
// Inspired by work of neal_wu

static int highest_bit(const int x) {
    return x == 0 ? -1 : 31 - __builtin_clz(x);
}

template<typename T, bool maximum_mode = false> // inspired by neal_wu
struct rmq_t {
    rmq_t(const std::vector<T>& v = {}) : _size((int)v.size()) {
        if (_size > 0) {
            _values = v;
            build();
        }
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

// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0200r0.html
template<class Fun> class y_combinator_result {
    Fun fun_;
public:
    template<class T> explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}
    template<class ...Args> decltype(auto) operator()(Args &&...args) { return fun_(std::ref(*this), std::forward<Args>(args)...); }
};
template<class Fun> decltype(auto) y_combinator(Fun &&fun) { return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun)); }

struct segment_change_t {

    segment_change_t(int64_t v = 0) : _value(v) {}

    void reset() { _value = 0; }
    bool has_change() const { return _value != 0; }

    // Return the combined result of applying this segment change followed by `other`.
    segment_change_t combine(const segment_change_t &other) const {
        return segment_change_t(_value + other._value);
    }

    int64_t _value;
};

struct segment_t {

    segment_t(int64_t v = 0) : _value(v) {}
    bool empty() const { return _value == 0; }
    void apply(int, const segment_change_t& change) { _value += change._value; }
    void join(const segment_t& other) { _value += other._value; }
    void join(const segment_t& a, const segment_t& b) { *this = a; join(b); }

    int64_t _value;
};

struct segtree_t {
    segtree_t(const int n = -1) {
        if (n >= 0)
            init(n);
    }

    void init(const int n) {
        _tree_n = 1;

        while (_tree_n < n)
            _tree_n *= 2;

        _tree.assign(2 * _tree_n, segment_t{});
        _changes.assign(_tree_n, segment_change_t{});
    }

    inline segment_t left(const int pos) { return _tree[2 * pos]; }
    inline segment_t right(const int pos) { return _tree[2 * pos + 1]; }

    // Builds our tree from an array in O(n).
    void build(const std::vector<segment_t>& initial) {
        const int n = int(initial.size());
        init(n);
        assert(n <= _tree_n);

        std::copy(initial.begin(), initial.end(), _tree.begin() + _tree_n);
        for (int pos = _tree_n - 1; pos > 0; --pos)
            _tree[pos].join(left(pos), right(pos));
    }

    void apply_and_combine(const int pos, const int length, const segment_change_t& change) {
        _tree[pos].apply(length, change);
        if (pos < _tree_n) _changes[pos] = _changes[pos].combine(change);
    }

    void push_down(const int pos, const int length) {
        auto& chg = _changes[pos];
        if (chg.has_change()) {
            apply_and_combine(2 * pos, length / 2, chg);
            apply_and_combine(2 * pos + 1, length / 2, chg);
            chg.reset();
        }
    }

    // Calls push_down for all necessary nodes in order to query the range [a, b).
    void push_all(int a, int b) {
        assert(0 <= a && a < b && b <= _tree_n);
        a += _tree_n;
        b += _tree_n - 1;

        for (int up = highest_bit(_tree_n); up > 0; --up) {
            const int x = a >> up, y = b >> up;
            push_down(x, 1 << up);
            if (x != y) push_down(y, 1 << up);
        }
    }

    void join_and_apply(const int pos, const int length) {
        _tree[pos].join(left(pos), right(pos));
        _tree[pos].apply(length, _changes[pos]);
    }

    // Calls join for all necessary nodes after updating the range [a, b).
    void join_all(int a, int b) {
        assert(0 <= a && a < b && b <= _tree_n);
        a += _tree_n;
        b += _tree_n - 1;
        int length = 1;

        while (a > 1) {
            a /= 2;
            b /= 2;
            length *= 2;
            join_and_apply(a, length);
            if (a != b) join_and_apply(b, length);
        }
    }

    template<typename T_range_op>
    void process_range(int a, int b, bool needs_join, T_range_op &&range_op) {
        if (a == b) return;
        push_all(a, b);
        const int original_a{a}, original_b{b};
        int length = 1, r_size = 0;

        for (a += _tree_n, b += _tree_n; a < b; a /= 2, b /= 2, length *= 2) {
            if (a & 1)
                range_op(a++, length);

            if (b & 1)
                _right_half[r_size++] = {--b, length};
        }

        for (int i = r_size - 1; i >= 0; --i)
            range_op(_right_half[i].first, _right_half[i].second);

        if (needs_join)
            join_all(original_a, original_b);
    }

    segment_t query(const int a, const int b) {
        assert(0 <= a && a <= b && b <= _tree_n);
        segment_t ans;

        process_range(a, b, false, [&](int pos, int) {
            ans.join(_tree[pos]);
        });

        return ans;
    }

    void update(const int a, const int b, const segment_change_t& change) {
        assert(0 <= a && a <= b && b <= _tree_n);

        process_range(a, b, true, [&](int pos, int length) {
            apply_and_combine(pos, length, change);
        });
    }

    std::vector<segment_t> to_array() {
        for (int i = 1; i < _tree_n; ++i)
            push_down(i, _tree_n >> highest_bit(i));

        std::vector<segment_t> ret(_tree_n);
        std::copy(_tree.begin() + _tree_n, _tree.end(), ret.begin());

        return ret;
    }

    void update_single(int index, const segment_t& seg) {
        assert(0 <= index && index < _tree_n);
        int pos = _tree_n + index;

        for (int up = highest_bit(_tree_n); up > 0; --up)
            push_down(pos >> up, 1 << up);

        _tree[pos] = seg;

        while (pos > 1) {
            pos /= 2;
            _tree[pos].join(left(pos), right(pos));
        }
    }

    // Finds the last subarray starting at `first` that satisifes `should_join` via binary search in O(log n).
    template<typename T_bool>
    int find_last_subarray(T_bool &&should_join, const int n, int first = 0) {
        assert(0 <= first && first <= n);
        segment_t current;

        // Check the degenerate case.
        if (!should_join(current, current))
            return first - 1;

        return y_combinator([&](auto search, const int pos, const int start, const int end) -> int {
            if (end <= first) {
                return end;
            } else if (first <= start && end <= n && should_join(current, _tree[pos])) {
                current.join(_tree[pos]);
                return end;
            } else if (end - start == 1) {
                return start;
            }

            push_down(pos, end - start);
            const int mid = (start + end) / 2;
            const int left = search(2 * pos, start, mid);
            return left < mid ? left : search(2 * pos + 1, mid, end);
        })(1, 0, _tree_n);
    }

    int _tree_n{0};
    std::pair<int, int> _right_half[32];
    std::vector<segment_t> _tree;
    std::vector<segment_change_t> _changes;
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

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    init();
    g_tree.build();
    //test();
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 segtree.cpp -o segtree.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address segtree.cpp -o segtree

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

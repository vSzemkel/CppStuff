
#include <algorithm>
#include <array>
#include <assert.h>
#include <iostream>
#include <numeric>
#include <optional>
#include <string>
#include <vector>

// Dependent Events
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000435914/00000000008d9970

template <int M = 998244353>
class modnum_t {
  public:
    modnum_t() : value(0) {}
    constexpr modnum_t(int64_t v) : value(v % M) {
        if (value < 0) value += M;
    }

    modnum_t inv() const
    {
        modnum_t res;
        res.value = minv(value, M);
        return res;
    }

    modnum_t neg() const
    {
        modnum_t res;
        res.value = value ? M - value : 0;
        return res;
    }

    modnum_t operator-() const
    {
        return neg();
    }

    modnum_t operator+() const
    {
        return modnum_t(*this);
    }

    modnum_t& operator+=(const modnum_t& o)
    {
        value += o.value;
        if (value >= M) value -= M;
        return *this;
    }

    modnum_t& operator-=(const modnum_t& o)
    {
        value -= o.value;
        if (value < 0) value += M;
        return *this;
    }

    modnum_t& operator*=(const modnum_t& o)
    {
        value = int(int64_t(value) * int64_t(o.value) % M);
        return *this;
    }

    modnum_t& operator/=(const modnum_t& o)
    {
        return *this *= o.inv();
    }

    modnum_t& operator++()
    {
        value++;
        if (value == M) value = 0;
        return *this;
    }

    modnum_t& operator--()
    {
        if (value == 0) value = M;
        value--;
        return *this;
    }

    friend modnum_t operator++(modnum_t& a, int)
    {
        modnum_t r = a;
        ++a;
        return r;
    }

    friend modnum_t operator--(modnum_t& a, int)
    {
        modnum_t r = a;
        --a;
        return r;
    }

    friend std::optional<modnum_t> inv(const modnum_t& m) { const auto ret = m.inv(); return ret.value < 0 ? std::nullopt : std::optional{ret}; }
    friend modnum_t neg(const modnum_t& m) { return m.neg(); }

    friend std::ostream& operator<<(std::ostream& out, const modnum_t& n) { return out << int(n); }
    friend std::istream& operator>>(std::istream& in, modnum_t& n) { int64_t v; in >> v; n = modnum_t(v); return in; }

    friend bool operator<(const modnum_t& a, const modnum_t& b) { return a.value < b.value; }
    friend bool operator==(const modnum_t& a, const modnum_t& b) { return a.value == b.value; }
    friend bool operator!=(const modnum_t& a, const modnum_t& b) { return a.value != b.value; }
    friend modnum_t operator+(const modnum_t& a, const modnum_t& b) { return modnum_t(a) += b; }
    friend modnum_t operator-(const modnum_t& a, const modnum_t& b) { return modnum_t(a) -= b; }
    friend modnum_t operator*(const modnum_t& a, const modnum_t& b) { return modnum_t(a) *= b; }
    friend modnum_t operator/(const modnum_t& a, const modnum_t& b) { return modnum_t(a) /= b; }
    friend int operator%(const modnum_t& a, const int b) { return a.value % b; }

    explicit operator int() const { return value; }

  private:
    static int minv(int a, const int m) {
        a %= m;
        if (a == 0) return -1; // not exists
        return a == 1 ? 1 : int(m - int64_t(minv(m, a)) * int64_t(m) / a);
    }

    int value;
};

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
        _lgsz = 0;
        int sz{n}; while (sz) { sz >>= 1; ++_lgsz; }
        _bl.assign(n, std::vector<int>(_lgsz));
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
        _depth[node] = ~par ? _depth[par] + 1 : 0;

        // Build data structure for binary lifting search
        _bl[node][0] = ~par ? par : node;
        for (int i = 1; i < _lgsz; ++i)
            _bl[node][i] = _bl[_bl[node][i - 1]][i - 1];

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

    // Note: returns -1 if `a` and `b` aren't connected or a == b are root.
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

    static constexpr int max_depth(int size) {
        int ret{0};
        while (size) {
            size >>= 1;
            ++ret;
        }
        return ret;
    }

  // private:
    int _size{};
    int _lgsz, _post_tour, _tour;
    std::vector<std::vector<int>> _adj, _bl;
    std::vector<int> _parent, _depth, _subtree_size;
    std::vector<int> _euler, _euler_first_rank;
    std::vector<int> _dfs_rank, _dfs_rank_end, _postorder;
    std::vector<int> _dfs_list, _rev_dfs_list;
    std::vector<int> _paths, _subtree_paths;
    std::vector<int> _heavy_root;
    rmq_t<int> _rmq;
    bool _built{};
};

constexpr const int M = 1000000007;
constexpr const int FACTOR = 1000000;
constexpr const int DEPTH = desctree_t::max_depth(200000);

using mod_t = modnum_t<M>;

static void solve() { // by tmwilliamlin168
    int N, Q;
    std::cin >> N >> Q;
    desctree_t tree(N);
    std::vector<mod_t> condp(N);
    std::vector<std::array<int, DEPTH>> anc(N);
    std::vector<std::array<mod_t, DEPTH>> succ(N), fail(N);
    const mod_t NORM = mod_t{1} / mod_t{FACTOR};
    anc[0].fill(-1); // root has no ancestors
    std::cin >> condp[0];
    condp[0] *= NORM;
    for (int i = 1; i < N; ++i) {
        int p;
        std::cin >> p >> succ[i][0] >> fail[i][0];
        succ[i][0] *= NORM;
        fail[i][0] *= NORM;
        anc[i][0] = --p;
        tree.add_edge(p, i);
        condp[i] = condp[p] * succ[i][0] + (1 - condp[p]) * fail[i][0];
        for (int k = 1; k < DEPTH; ++k) {
            anc[i][k] = ~anc[i][k - 1] ? anc[anc[i][k - 1]][k - 1] : -1;
            if (~anc[i][k]) {
                succ[i][k] = succ[i][k - 1] * succ[anc[i][k - 1]][k - 1] + fail[i][k - 1] * (1 - succ[anc[i][k - 1]][k - 1]);
                fail[i][k] = succ[i][k - 1] * fail[anc[i][k - 1]][k - 1] + fail[i][k - 1] * (1 - fail[anc[i][k - 1]][k - 1]);
            }
        }
    }

    tree.build();
    for (int q = 0; q < Q; ++q) {
        int p, r; std::cin >> p >> r;
        --p; --r;

        const int la = tree.get_lca(p, r);
        std::array<mod_t, 2> ap{1, 0}, ar{1, 0}; // {happens when parent, happens when not parent}
        auto agg = [&](int& u, int k, std::array<mod_t, 2>& a) {
            const auto& s = succ[u][k], f = fail[u][k];
            a = {a[0] * s + a[1] * (1 - s), a[0] * f + a[1] * (1 - f)};
            u = anc[u][k];
        };

        for (int k = DEPTH - 1; k >= 0; --k) {
            while (~anc[p][k] && la < p && la <= anc[p][k]) // in-tree && below-target && parent-not-above-target
                agg(p, k, ap);
            while (~anc[r][k] && la < r && la <= anc[r][k])
                agg(r, k, ar);
        }

        const auto ans = ap[0] * ar[0] * condp[la] + ap[1] * ar[1] * (1 - condp[la]);
        std::cout << ans << ' ';
    }
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 dependent_events.cpp -o dependent_events.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address dependent_events.cpp -o dependent_events

Run:
dependent_events.exe < dependent_events.in

Input:

2
5 2
200000
1 400000 300000
2 500000 200000
1 800000 100000
4 200000 400000
1 5
3 5
4 2
300000
1 100000 100000
2 300000 400000
3 500000 600000
1 2
2 4

Output:


*/

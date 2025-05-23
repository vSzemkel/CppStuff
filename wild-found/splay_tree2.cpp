#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <random>
#include <set>
#include <vector>
using namespace std;

// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0200r0.html
template<class Fun> class y_combinator_result {
    Fun fun_;
public:
    template<class T> explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}
    template<class ...Args> decltype(auto) operator()(Args &&...args) { return fun_(std::ref(*this), std::forward<Args>(args)...); }
};
template<class Fun> decltype(auto) y_combinator(Fun &&fun) { return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun)); }


template<typename A, typename B> ostream& operator<<(ostream &os, const pair<A, B> &p) { return os << '(' << p.first << ", " << p.second << ')'; }
template<typename... Args> ostream& operator<<(ostream& os, const tuple<Args...>& t) { os << '('; apply([&os](const Args&... args) { size_t n = 0; ((os << args << (++n != sizeof...(Args) ? ", " : "")), ...); }, t); return os << ')'; }
template<typename T_container, typename T = typename enable_if<!is_same<T_container, string>::value, typename T_container::value_type>::type> ostream& operator<<(ostream &os, const T_container &v) { os << '{'; string sep; for (const T &x : v) os << sep << x, sep = ", "; return os << '}'; }

void dbg_out() { cerr << endl; }
template<typename Head, typename... Tail> void dbg_out(Head H, Tail... T) { cerr << ' ' << H; dbg_out(T...); }
#ifdef NEAL_DEBUG
#define dbg(...) cerr << '[' << __FILE__ << ':' << __LINE__ << "] (" << #__VA_ARGS__ << "):", dbg_out(__VA_ARGS__)
#else
#define dbg(...)
#endif

// Note that T must be able to handle sums of values, not just individual values.
using T = int64_t;

struct splay_change {
    T to_add;

    splay_change(T _to_add = 0) : to_add(_to_add) {}

    bool has_change() const {
        return to_add != 0;
    }

    // Return the combined result of applying this splay_change followed by `other`.
    splay_change combine(const splay_change &other) const {
        return splay_change(to_add + other.to_add);
    }
};

struct splay_node {
    splay_node *parent = nullptr, *child[2] = {nullptr, nullptr};
    int size = 1;

    T value, maximum, sum;
    splay_change change;

    friend int get_size(splay_node *x) {
        return x == nullptr ? 0 : x->size;
    }

    friend T get_max(splay_node *x) {
        return x == nullptr ? numeric_limits<T>::lowest() : x->maximum;
    }

    friend T get_sum(splay_node *x) {
        return x == nullptr ? 0 : x->sum;
    }

    int parent_index() const {
        return parent == nullptr ? -1 : int(this == parent->child[1]);
    }

    void set_child(int index, splay_node *x) {
        child[index] = x;

        if (x != nullptr)
            x->parent = this;
    }

    void apply_and_combine(const splay_change &now) {
        value += now.to_add;
        sum += size * now.to_add;
        maximum += now.to_add;

        change = change.combine(now);
    }

    void push() {
        if (change.has_change()) {
            if (child[0] != nullptr) child[0]->apply_and_combine(change);
            if (child[1] != nullptr) child[1]->apply_and_combine(change);
            change = splay_change();
        }
    }

    void join() {
        size = get_size(child[0]) + get_size(child[1]) + 1;
        // TODO: Add other operations here. Remember to store my_value in addition to, e.g., min_value or max_value.
        sum = value + get_sum(child[0]) + get_sum(child[1]);
        maximum = max({value, get_max(child[0]), get_max(child[1])});
    }
};

int64_t splay_count = 0;

struct splay_tree {
    static const int POOL_SIZE = 10000;
    static vector<splay_node*> node_pool;
    static vector<splay_node*> pointers_to_delete;

    static splay_node* new_node(const T &value) {
        if (node_pool.empty()) {
            splay_node *ptr = new splay_node[POOL_SIZE];
            pointers_to_delete.push_back(ptr);
            node_pool.reserve(POOL_SIZE);

            for (int i = POOL_SIZE - 1; i >= 0; i--)
                node_pool.push_back(ptr + i);
        }

        splay_node *node = node_pool.back();
        node_pool.pop_back();
        *node = splay_node();
        node->value = value;
        // TODO: add more logic for creating a new node here.
        node->join();
        return node;
    }

    static bool _exit_delete_setup;

    static void _delete_pointers() {
        for (splay_node *node : pointers_to_delete)
            delete[] node;

        pointers_to_delete.clear();
    }

    ~splay_tree() {
        if (!_exit_delete_setup) {
            atexit(_delete_pointers);
            _exit_delete_setup = true;
        }
    }

    splay_node *root = nullptr;

    splay_tree(const vector<T> &values = {}) {
        init(values);
    }

    splay_tree(splay_node *node) {
        set_root(node);
    }

    splay_node *construct(const vector<T> &values, int start, int end) {
        if (start >= end)
            return nullptr;

        if (end - start == 1)
            return new_node(values[start]);

        int mid = (start + end) / 2;
        splay_node *current = new_node(values[mid]);
        current->set_child(0, construct(values, start, mid));
        current->set_child(1, construct(values, mid + 1, end));
        current->join();
        return current;
    }

    // Constructs the splay tree in linear time if the values are already sorted.
    void init(vector<T> values) {
        set_root(construct(values, 0, int(values.size())));
    }

    bool empty() const {
        return root == nullptr;
    }

    int size() const {
        return get_size(root);
    }

    splay_node *set_root(splay_node *x) {
        if (x != nullptr)
            x->parent = nullptr;

        return root = x;
    }

    void rotate_up(splay_node *x, bool x_join = true) {
        splay_node *p = x->parent, *gp = p->parent;
        int index = x->parent_index();

        if (gp == nullptr)
            set_root(x);
        else
            gp->set_child(p->parent_index(), x);

        p->set_child(index, x->child[!index]);
        x->set_child(!index, p);
        p->join();

        if (x_join)
            x->join();
    }

    // Note that splay(x) handles both pushing x and joining all nodes from x to the root, inclusive.
    void splay(splay_node *x) {
        splay_count++;
        x->push();

        while (x != root) {
            splay_node *p = x->parent;

            if (p != root)
                rotate_up(x->parent_index() == p->parent_index() ? p : x, false);

            rotate_up(x, false);
        }

        x->join();
    }

    splay_node *node_at_index(int index) {
        if (index < 0 || index >= size())
            return nullptr;

        splay_node *current = root;

        while (current != nullptr) {
            current->push();
            int left_size = get_size(current->child[0]);

            if (index == left_size) {
                splay(current);
                return current;
            }

            if (index < left_size) {
                current = current->child[0];
            } else {
                current = current->child[1];
                index -= left_size + 1;
            }
        }

        assert(false);
        return nullptr;
    }

    splay_node *insert(int index, const T &value) {
        return insert(index, new_node(value));
    }

    splay_node *insert(int index, splay_node *node) {
        assert(0 <= index && index <= size());

        if (node == nullptr)
            return nullptr;
        else if (root == nullptr)
            return set_root(node);

        splay_node *current = root, *previous = nullptr;
        int previous_dir = -1;

        while (current != nullptr) {
            current->push();
            previous = current;
            int left_size = get_size(current->child[0]);

            if (index <= left_size) {
                current = current->child[0];
                previous_dir = 0;
            } else {
                current = current->child[1];
                previous_dir = 1;
                index -= left_size + 1;
            }
        }

        previous->set_child(previous_dir, node);
        splay(node);
        return node;
    }

    splay_node *begin() {
        if (root == nullptr)
            return nullptr;

        splay_node *x = root;
        x->push();

        while (x->child[0] != nullptr) {
            x = x->child[0];
            x->push();
        }

        splay(x);
        return x;
    }

    // To iterate through all nodes in order:
    // for (splay_node *node = tree.begin(); node != nullptr; node = tree.successor(node))
    splay_node *successor(splay_node *x) const {
        if (x == nullptr)
            return nullptr;

        x->push();

        if (x->child[1] != nullptr) {
            x = x->child[1];
            x->push();

            while (x->child[0] != nullptr) {
                x = x->child[0];
                x->push();
            }

            return x;
        }

        while (x->parent_index() == 1)
            x = x->parent;

        return x->parent;
    }

    splay_node *predecessor(splay_node *x) const {
        if (x == nullptr)
            return nullptr;

        x->push();

        if (x->child[0] != nullptr) {
            x = x->child[0];
            x->push();

            while (x->child[1] != nullptr) {
                x = x->child[1];
                x->push();
            }

            return x;
        }

        while (x->parent_index() == 0)
            x = x->parent;

        return x->parent;
    }

    splay_node *last() {
        if (root == nullptr)
            return nullptr;

        splay_node *x = root;
        x->push();

        while (x->child[1] != nullptr) {
            x = x->child[1];
            x->push();
        }

        splay(x);
        return x;
    }

    void clear() {
        vector<splay_node*> nodes;
        nodes.reserve(size());

        for (splay_node *node = begin(); node != nullptr; node = successor(node))
            nodes.push_back(node);

        for (splay_node *node : nodes) {
            // Instead of deleting, add `node` back to `node_pool`.
            *node = splay_node();
            node_pool.push_back(node);
        }

        set_root(nullptr);
    }

    void erase(splay_node *x) {
        splay(x);

        if (x->child[0] == nullptr || x->child[1] == nullptr) {
            set_root(x->child[int(x->child[0] == nullptr)]);
        } else {
            set_root(x->child[0]);
            insert(size(), x->child[1]);
        }

        // Instead of deleting, add `x` back to `node_pool`.
        *x = splay_node();
        node_pool.push_back(x);
    }

    // Detach x from its parent, producing two separate splay trees as a result.
    void detach(splay_node *x) {
        if (x == nullptr)
            return;

        if (x == root) {
            set_root(nullptr);
            return;
        }

        splay_node *parent = x->parent;
        assert(parent != nullptr);
        parent->set_child(x->parent_index(), nullptr);
        x->parent = nullptr;
        splay(parent);
        x->push();
    }

    // Returns a splay_node pointer representing the first `count` nodes. If none, returns `nullptr`.
    splay_node *query_prefix_count(int count) {
        if (count <= 0)
            return nullptr;
        else if (count >= size())
            return root;

        splay_node *node = node_at_index(count);
        splay(node);
        return node->child[0];
    }

    // Returns a splay_node pointer representing the last `count` nodes. If none, returns `nullptr`.
    splay_node *query_suffix_count(int count) {
        if (count <= 0)
            return nullptr;
        else if (count >= size())
            return root;

        int index = size() - count;
        splay_node *node = node_at_index(index - 1);
        splay(node);
        return node->child[1];
    }

    // Returns a splay_node pointer representing the index range [start, end). If none, returns `nullptr`.
    splay_node *query_range(int start, int end) {
        if (start >= end)
            return nullptr;
        else if (start <= 0)
            return query_prefix_count(end);
        else if (end >= size())
            return query_suffix_count(size() - start);

        splay_node *before = node_at_index(start - 1);
        splay_node *after = node_at_index(end);
        splay(after);
        splay(before);

        if (after->parent != before)
            rotate_up(after);

        assert(before->child[1] == after);
        return after->child[0];
    }

    // Applies an update to the subtree rooted at `node`.
    void update(splay_node *node, const splay_change &change) {
        if (node == nullptr)
            return;

        node->apply_and_combine(change);
        splay(node);
    }

    // Finds the end of the last subarray starting at `first` satisfying `should_join` via binary search.
    // should_join(splay_node *node, bool single_node) -> bool
    // Determines whether we should join with a node (if single_node then just the node, else the subtree).
    // If true, actually performs the join.
    template<typename T_bool>
    int find_last_subarray(T_bool &&should_join, int first = 0) {
        if (!should_join(nullptr, false))
            return first - 1;

        splay_node *current = first == 0 ? root : query_suffix_count(size() - first);
        splay_node *previous = nullptr;
        int end = first;

        while (current != nullptr) {
            current->push();
            previous = current;

            if (!should_join(current->child[0], false)) {
                current = current->child[0];
            } else {
                end += get_size(current->child[0]);

                if (!should_join(current, true))
                    break;

                end++;
                current = current->child[1];
            }
        }

        if (previous != nullptr)
            splay(previous);

        return end;
    }
};

bool splay_tree::_exit_delete_setup = false;
vector<splay_node*> splay_tree::node_pool;
vector<splay_node*> splay_tree::pointers_to_delete;


const int EG_MAX = int(2e6) + 5;

void run_case(int test_case) {
    int N, G;
    cin >> N >> G;
    vector<int> E(N);

    for (auto &e : E)
        cin >> e;

    splay_tree tree;

    for (auto &e : E) {
        int low = 0, high = EG_MAX;

        while (low < high) {
            int mid = low + (high - low + 1) / 2;

            int below = tree.find_last_subarray([&](splay_node *node, bool single_node) -> bool {
                return (single_node ? node->value : get_max(node)) < mid;
            });

            if (mid - below <= e)
                low = mid;
            else
                high = mid - 1;
        }

        int below = tree.find_last_subarray([&](splay_node *node, bool single_node) -> bool {
            return (single_node ? node->value : get_max(node)) < low;
        });
        tree.update(tree.query_prefix_count(below), splay_change(-1));
        tree.insert(below, low);
    }

    int closest = 2 * EG_MAX, best_index = -1;
    int index = 0;

    for (splay_node *node = tree.begin(); node != nullptr; node = tree.successor(node), index++) {
        int dist = abs(int(node->value) - G);

        if (dist <= closest) {
            closest = dist;
            best_index = index;
        }
    }

    cout << "Case #" << test_case << ": " << N - best_index << ' ' << closest << '\n';
}

int main() {
    int tests;
    cin >> tests;

    for (int tc = 1; tc <= tests; tc++) {
        run_case(tc);
        cout << flush;
    }
}


clang++.exe -Wall -Wextra -g -O0 -std=c++20 line_of_delivery_part_2_neal_wu_source_code.cpp -o line_of_delivery_b.exe
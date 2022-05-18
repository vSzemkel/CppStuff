
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Diverse Subarray
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050eda/00000000001198c1#problem


template <typename T = int64_t>
struct updatable_segment_tree_t
{
  public:
    updatable_segment_tree_t(const int size) : _size(size), _changes(size << 2), _nodes(size << 2) {
        _build(1, 0, _size);
    }

    T query(const int lower, const int upper) { // range [lower, upper)
        return _query(lower, upper, 1, 0, _size);
    }

    void update(const int lower, const int upper, const T delta) {
        _update(lower, upper, delta, 1, 0, _size);
    }

    static inline const T SEED{0}; // TO DO: change value accordingly

  private:
    static T join(const T& lhs, const T& rhs) {
        return std::max(lhs, rhs); // TO DO: change join implementation accordingly
    }

    inline void upd(const int id, const T delta) {
        _changes[id] += delta;
        _nodes[id] += delta;
    }

    inline void push(const int id, const int left, const int right) {
        if (_changes[id] == 0)
            return;
        upd(left, _changes[id]);
        upd(right, _changes[id]);
        _changes[id] = 0;
    }

    void _build(const int root, const int left, const int right) {
        _changes[root] = 0;
        _nodes[root] = SEED;
        if (right - left < 2)
            return ;

        const int mid = left + ((right - left) >> 1), il = root << 1, ir = il | 1;
        _build(il, left, mid);
        _build(ir, mid, right);
    }

    T _query(const int lower, const int upper, const int root, const int l, const int r) { // range [lower, upper)
        if (lower >= r || l >= upper)
            return 0;
        if (lower <= l && r <= upper)
            return _nodes[root];

        const int mid = l + ((r - l) >> 1), il = root << 1, ir = il | 1;
        push(root, il, ir);
        return join(_query(lower, upper, il, l, mid), _query(lower, upper, ir, mid, r));
    }

    void _update(const int lower, const int upper, const T delta, const int root, const int l, const int r) {
        if (lower >= r || l >= upper)
            return;
        if (lower <= l && r <= upper) {
            upd(root, delta);
            return;
        }
        const int mid = l + ((r - l) >> 1), il = root << 1, ir = il | 1;
        push(root, il, ir);
        _update(lower, upper, delta, il, l, mid);
        _update(lower, upper, delta, ir, mid, r);
        _nodes[root] = join(_nodes[il], _nodes[ir]);
    }

    int _offset{1}, _size;
    std::vector<T> _changes, _nodes;
};

static void solve() { // O(NlogN)
    int N;
    size_t S;
    std::cin >> N >> S;

    int64_t ans{0};
    updatable_segment_tree_t ust(N); // ust._nodes[k] = sum[k..i]
    std::unordered_map<int, std::vector<int>> pos; // type -> sorted list of positions
    for (int i = 0; i < N; ++i) {
        int t; std::cin >> t;
        auto& p = pos[t];
        p.push_back(i);
        int l{0};
        if (S < p.size())
            l = p[p.size() - S - 1] + 1;
        ust.update(l, i + 1, +1);
        if (S < p.size()) {
            l = 0;
            if (S + 1 < p.size())
                l = p[p.size() - S - 2] + 1;
            const int r = p[p.size() - S - 1] + 1;
            ust.update(l, r, -S);
        }
        ans = std::max(ans, ust.query(0, i + 1));
    }

    std::cout << ans;
}

static void solve_set1() { // O(N2)
    int N;
    size_t S;
    std::cin >> N >> S;
    std::vector<int> trinkets(N);
    for (auto& t :trinkets)
        std::cin >> t;

    size_t ans{0};
    for (int i = 0; i < N; ++i) {
        std::unordered_set<int> omit;
        std::unordered_multiset<int> cur;
        for (int j = i; j < N; ++j) {
            const int t = trinkets[j];
            if (omit.count(t))
                continue;
            if (cur.count(t) < S)
                cur.insert(t);
            else {
                cur.erase(t);
                omit.insert(t);
            }

            ans = std::max(ans, cur.size());
        }
    }

    std::cout << ans;
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
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 diverse_subarray.cpp -o diverse_subarray.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address diverse_subarray.cpp -o diverse_subarray

Run:
diverse_subarray.exe < diverse_subarray.in

Input:

4
6 2
1 1 4 1 4 4
8 1
1 2 500 3 4 500 6 7
10 1
100 200 8 8 8 8 8 300 400 100
12 2
40 50 1 1 1 60 70 2 2 2 80 90

Output:

Case #1: 4
Case #2: 6
Case #3: 4
Case #4: 6

*/

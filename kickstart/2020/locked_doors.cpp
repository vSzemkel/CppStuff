
#include <array>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <optional>
#include <queue>
#include <utility>
#include <vector>

// Locked Doors
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ff08/0000000000386d5c

std::vector<std::vector<int>> ancestors, root_path;
std::vector<std::array<int, 2>> treap;
std::vector<int> doors, sizes, parent;

static void solve() {
    int N, Q; std::cin >> N >> Q;
    doors.resize(N - 1);
    for (auto& d : doors)
        std::cin >> d;
    // build orber table and normalize
    std::vector<int> doors_ord(N - 1);
    std::iota(doors_ord.begin(), doors_ord.end(), 0);
    std::sort(doors_ord.begin(), doors_ord.end(), [](const int i, const int j){
        return doors[i] < doors[j];
    });
    for (int i = 0; i < N - 1; ++i)
        doors[doors_ord[i]] = i + 1;
    // find next greater neigbor
    std::vector<int> left_greater(N - 1), right_greater(N - 1), stack;
    for (int i = 0; i < N - 1; ++i) {
        while (!stack.empty() && doors[stack.back()] < doors[i]) {
            right_greater[stack.back()] = i;
            stack.pop_back();
        }
        stack.push_back(i);
    }
    while (!stack.empty()) {
        right_greater[stack.back()] = -1;
        stack.pop_back();
    }
    for (int i = N - 2; i >= 0; --i) {
        while (!stack.empty() && doors[stack.back()] < doors[i]) {
            left_greater[stack.back()] = i;
            stack.pop_back();
        }
        stack.push_back(i);
    }
    while (!stack.empty()) {
        left_greater[stack.back()] = -1;
        stack.pop_back();
    }
    // initialize ancestors
    int lg2size{0}, counter{N - 1};
    const int root = doors_ord.back();
    while (counter > 0) { counter >>= 1; ++lg2size; }
    ancestors.assign(N - 1, std::vector<int>(lg2size, root));
    // build the treap
    treap.assign(N - 1, {-1, -1});
    for (int i = 0; i < N - 1; ++i) {
        const int l = left_greater[i];
        const int r = right_greater[i];
        if (l < 0 && r >= 0) {
            treap[r][0] = i;
            ancestors[i][0] = r;
        }
        if (r < 0 && l >= 0) {
            treap[l][1] = i;
            ancestors[i][0] = l;
        }
        if (l >= 0 && r >= 0) {
            if (doors[l] < doors[r]) {
                treap[l][1] = i;
                ancestors[i][0] = l;
            } else {
                treap[r][0] = i;
                ancestors[i][0] = r;
            }
        }
    }
    // fill ancestors
    for (int j = 1; j < lg2size; ++j)
        for (int i = 0; i < N - 1; ++i)
            ancestors[i][j] = ancestors[ancestors[i][j - 1]][j - 1];
    // compute sizes
    sizes.resize(N - 1);
    for (const int d : doors_ord) {
        const auto& tnode = treap[d];
        sizes[d] = 1 + (tnode[0] < 0 ? 0 : sizes[tnode[0]]) + (tnode[1] < 0 ? 0 : sizes[tnode[1]]);
    }

    doors.push_back(std::numeric_limits<int>::max());
    for (int q = 0; q < Q; ++q) {
        int S, K; std::cin >> S >> K; --S;
        int ans{S};
        if (--K > 0) {
            bool right{true};
            int first_door{S};
            if (S > 0 && doors[S - 1] < doors[S]) {
                right = false;
                --first_door;
            }
            if (K <= sizes[first_door])
                ans = S + (right ? K : -K);
            else {
                int query_root = first_door, len = lg2size - 1;
                while (true) {
                    const int candidate = ancestors[query_root][len];
                    if (sizes[candidate] < K)
                        query_root = candidate;
                    if (len == 0)
                        break;
                    --len;
                }
                query_root = ancestors[query_root][0];
                if (first_door < query_root) // coming from left subtree
                    ans = query_root + K - sizes[treap[query_root][0]];
                else
                    ans = query_root - (K - sizes[treap[query_root][1]]) + 1;
            }
        }

        std::cout << ' ' << ans + 1;
    }
}

namespace std {
    template <class Fun>
    class y_combinator_result
    {
        Fun fun_;

    public:
        template <class T>
        explicit y_combinator_result(T&& fun) : fun_(std::forward<T>(fun)) {}

        template <class... Args>
        decltype(auto) operator()(Args&&... args) {
            return fun_(std::ref(*this), std::forward<Args>(args)...);
        }
    };

    template <class Fun>
    decltype(auto) y_combinator(Fun&& fun) {
        return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
    }
} // namespace std

static void solve_ecnerwala() {
    int N, Q; std::cin >> N >> Q;
    doors.resize(N - 1);
    for (auto& d : doors)
        std::cin >> d;
    std::vector<int> answers(Q);
    std::vector<std::vector<std::pair<int, int>>> queries(N);
    for (int q = 0; q < Q; ++q) {
        int S, K; std::cin >> S >> K; --S; --K;
        queries[S].emplace_back(K, q);
    }

    // build the cartesian tree
    std::vector<std::pair<int, int>> seg(2 * (N - 1));
    for (int i = 0; i < N - 1; i++)
        seg[N - 1 + i] = {doors[i], i};
    for (int i = N - 2; i; i--)
        seg[i] = max(seg[2 * i], seg[2 * i + 1]);

    auto query_seg = [&](int l, int r) -> std::pair<int, int> {
        assert(l < r);
        std::pair<int, int> ans{-1, -1};
        for (int a = N - 1 + l, b = N - 1 + r; a < b; a /= 2, b /= 2) {
            if (a & 1) ans = std::max(ans, seg[a++]);
            if (b & 1) ans = std::max(ans, seg[--b]);
        }
        return ans;
    };

    using pq = std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>>;
    std::y_combinator([&](auto self, int l, int r) -> pq {
        assert(l < r);
        const int len = r - l;
        if (len == 1) {
            pq resp(queries[l].begin(), queries[l].end());
            while (!resp.empty() && resp.top().first == 0) {
                answers[resp.top().second] = l;
                resp.pop();
            }
            return resp;
        }

        const int m = query_seg(l, r - 1).second + 1;
        assert(l < m && m < r);
        pq l_resp = self(l, m);
        while (!l_resp.empty() && l_resp.top().first < len) {
            answers[l_resp.top().second] = l + l_resp.top().first;
            l_resp.pop();
        }

        pq r_resp = self(m, r);
        while (!r_resp.empty() && r_resp.top().first < len) {
            answers[r_resp.top().second] = r - 1 - r_resp.top().first;
            r_resp.pop();
        }

        if (l_resp.size() > r_resp.size()) swap(l_resp, r_resp);
        while (!l_resp.empty()) {
            r_resp.push(l_resp.top());
            l_resp.pop();
        }
        return r_resp;
    })(0, N);

    for (int q = 0; q < Q; q++)
        std::cout << ' ' << answers[q] + 1;
}

static void solve_mle() {
    int N, Q; std::cin >> N >> Q;
    doors.resize(N - 1);
    for (auto& d : doors)
        std::cin >> d;
    // build orber table and normalize
    std::vector<int> doors_ord(N - 1);
    std::iota(doors_ord.begin(), doors_ord.end(), 0);
    std::sort(doors_ord.begin(), doors_ord.end(), [](const int i, const int j){
        return doors[i] < doors[j];
    });
    for (int i = 0; i < N - 1; ++i)
        doors[doors_ord[i]] = i + 1;
    // find next greater neigbor
    std::vector<int> left_greater(N - 1), right_greater(N - 1), stack;
    for (int i = 0; i < N - 1; ++i) {
        while (!stack.empty() && doors[stack.back()] < doors[i]) {
            right_greater[stack.back()] = i;
            stack.pop_back();
        }
        stack.push_back(i);
    }
    while (!stack.empty()) {
        right_greater[stack.back()] = -1;
        stack.pop_back();
    }
    for (int i = N - 2; i >= 0; --i) {
        while (!stack.empty() && doors[stack.back()] < doors[i]) {
            left_greater[stack.back()] = i;
            stack.pop_back();
        }
        stack.push_back(i);
    }
    while (!stack.empty()) {
        left_greater[stack.back()] = -1;
        stack.pop_back();
    }
    // build the treap
    parent.resize(N - 1, -1);
    treap.assign(N - 1, {-1, -1});
    for (int i = 0; i < N - 1; ++i) {
        const int l = left_greater[i];
        const int r = right_greater[i];
        if (l < 0 && r >= 0) {
            treap[r][0] = i;
            parent[i] = r;
        }
        if (r < 0 && l >= 0) {
            treap[l][1] = i;
            parent[i] = l;
        }
        if (l >= 0 && r >= 0) {
            if (doors[l] < doors[r]) {
                treap[l][1] = i;
                parent[i] = l;
            } else {
                treap[r][0] = i;
                parent[i] = r;
            }
        }
    }
    // compute sizes
    sizes.resize(N - 1);
    for (const int d : doors_ord) {
        const auto& tnode = treap[d];
        sizes[d] = 1 + (tnode[0] < 0 ? 0 : sizes[tnode[0]]) + (tnode[1] < 0 ? 0 : sizes[tnode[1]]);
    }
    // and paths
    root_path.assign(N - 1, {});
    const int root = doors_ord.back();
    root_path[root].push_back(root);
    for (int i = N - 3; i >= 0; --i) {
        const int node = doors_ord[i];
        auto& rp = root_path[node];
        const auto& parent_path = root_path[parent[node]];
        rp.push_back(node);
        rp.insert(rp.end(), parent_path.begin(), parent_path.end());
    }

    for (int q = 0; q < Q; ++q) {
        int S, K; std::cin >> S >> K; --S;
        int ans{S};
        if (--K > 0) {
            bool right{true};
            int first_door{S};
            if (S > 0 && doors[S - 1] < doors[S]) {
                right = false;
                --first_door;
            }
            if (K <= sizes[first_door])
                ans = S + (right ? K : -K);
            else {
                const auto& path = root_path[first_door];
                const auto it = std::lower_bound(path.begin(), path.end(), K, [](const int n1, const int k){
                    return sizes[n1] < k;
                });
                assert(it != path.end());
                const auto query_root = *it;
                if (first_door < query_root) // coming from left subtree
                    ans = query_root + K - sizes[treap[query_root][0]];
                else
                    ans = query_root - (K - sizes[treap[query_root][1]]) + 1;
            }
        }

        std::cout << ' ' << ans + 1;
    }
}

template <typename K, typename V>
struct cache_t {
    void add(const K& key, const V& value) {
        _cache[key] = value;
    }

    std::optional<V> find(const K& key) {
        auto cached = _cache.find(key);
        if (cached == _cache.end())
            return std::nullopt;
        return cached->second;
    }
  private:
    std::map<K, V> _cache;
};

static void solve_set1_faster() {
    int N, Q; std::cin >> N >> Q;
    std::vector<int> doors(N - 1);
    for (auto& d : doors)
        std::cin >> d;
    doors.push_back(std::numeric_limits<int>::max());

    std::vector<int> left_greater(N), right_greater(N), stack;
    for (int i = 0; i < N; ++i) {
        while (!stack.empty() && doors[stack.back()] < doors[i]) {
            right_greater[stack.back()] = i;
            stack.pop_back();
        }
        stack.push_back(i);
    }
    while (!stack.empty()) {
        right_greater[stack.back()] = -1;
        stack.pop_back();
    }
    for (int i = N - 1; i >= 0; --i) {
        while (!stack.empty() && doors[stack.back()] < doors[i]) {
            left_greater[stack.back()] = i;
            stack.pop_back();
        }
        stack.push_back(i);
    }
    while (!stack.empty()) {
        left_greater[stack.back()] = -1;
        stack.pop_back();
    }

    cache_t<std::pair<int, int>, int> cache;
    for (int q = 0; q < Q; ++q) {
        int S, K; std::cin >> S >> K; --S;
        const auto cached = cache.find({S, K});
        if (cached != std::nullopt) {
            std::cout << ' ' << cached.value();
            continue;
        }

        int r = S;
        int l = S - 1;
        int ans{S}, counter{K};
        while (--counter > 0)
            if (l >= 0 && doors[l] < doors[r]) {
                int bound = (r > N - 1) ? l - 1 : left_greater[r];
                if (bound == -1) bound = -1;
                if (l - bound > counter) {
                    ans = l - counter + 1;
                    break;
                }
                counter -= l - bound - 1;
                ans = bound + 1;
                l = bound;
            } else {
                int bound = (l < 0) ? r + 1 : right_greater[l];
                if (bound == -1) bound = N;
                if (bound - r > counter) {
                    ans = r + counter;
                    break;
                }
                counter -= bound - r - 1;
                r = bound;
                ans = r;
            }

        ++ans;
        std::cout << ' ' << ans;
        cache.add({S, K}, ans);
    }
}

static void solve_set1() {
    int N, Q; std::cin >> N >> Q;
    std::vector<int> doors(N - 1);
    for (auto& d : doors)
        std::cin >> d;
    doors.push_back(std::numeric_limits<int>::max());

    for (int q = 0; q < Q; ++q) {
        int S, K; std::cin >> S >> K; --S;
        int ans{S};
        int l = S - 1;
        int r = S;
        while (--K > 0) {
            if (l >= 0 && doors[l] < doors[r]) {
                ans = l;
                --l;
            } else {
                ++r;
                ans = r;
            }
        }

        std::cout << ' ' << ans + 1;
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
        std::cout << "Case #" << g << ":"; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 locked_doors.cpp -o locked_doors.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address locked_doors.cpp -o locked_doors

Run:
locked_doors.exe < locked_doors.in

Input:

3
5 4
90 30 40 60
3 4
3 1
1 5
4 3
10 2
6 2 4 5 9 30 7 1 8
6 8
6 8
14 3
3 8 4 11 9 5 1 13 12 6 10 7 2
12 2
1 6
6 5

Output:

Case #1: 5 3 5 2
Case #2: 8 8
Case #3: 13 6 4

*/

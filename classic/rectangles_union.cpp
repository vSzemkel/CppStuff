
#include <algorithm>
#include <cassert>
#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>

// Rectangles union
// This solution uses variant of segment tree data structure

enum class event_t {
    start = 1,
    finish = -1
};

using sweep_t = std::tuple<int, event_t, int, int>; // <Y, TYPE, X1, X2>

struct segment_tree_node {
    int node_weight;    // for leafs
    int tree_weight;    // for subtrees
    int times_covered;  // leaf section covered times
};

struct cover_query {
  public:
    cover_query(const std::vector<int>& w) {
        const int len = (int)w.size();
        while (offset < len) offset *= 2;
        nodes.resize(2 * offset);
        for (int i = 0; i < len; ++i)
            nodes[offset + i].node_weight = w[i];
        for (int i = offset - 1; i > 0; --i)
            nodes[i].node_weight = nodes[2 * i].node_weight + nodes[2 * i + 1].node_weight;
    }

    int range_query(int l, int r) const { // range [l, r)
        int ret{0};
        for (l += offset, r += offset; l < r; l >>= 1, r >>= 1) { 
            if (l & 1)
                ret += nodes[l++].node_weight; 
            if (r & 1)  
                ret += nodes[--r].node_weight; 
        }

        return ret;
    }

    void change(int begin, int end, event_t ev) {
        _change(1, 0, offset, begin, end, (int)ev);
    }

    int query() const { return nodes[1].tree_weight; }

  private:
    int offset{1};
    std::vector<segment_tree_node> nodes;

    void _change(int root, int start, int span, int begin, int end, int change) {
        if (start + span <= begin || end <= start) // disjoint
            return;
        if (begin <= start && start + span <= end) // included
            nodes[root].times_covered += change;
        else {
            _change(2 * root, start, span / 2, begin, end, change);
            _change(2 * root + 1, start + span / 2, span / 2, begin, end, change);
        }

        if (nodes[root].times_covered == 0) {
            if (root >= offset) // leaf
                nodes[root].tree_weight = 0;
            else
                nodes[root].tree_weight = nodes[2 * root].tree_weight + nodes[2 * root + 1].tree_weight;
        } else
            nodes[root].tree_weight = nodes[root].node_weight;
    }
};

int64_t solve() {
    int N; std::cin >> N;
    std::vector<int> X;
    std::vector<sweep_t> edges(2 * N);
    for (int i = 0; i < N; ++i) {
        int x1, y1, x2, y2; std::cin >> x1 >> y1 >> x2 >> y2;
        assert(x1 < x2 && y1 < y2);
        X.push_back(x1);
        X.push_back(x2);
        edges[2 * i] = {y1, event_t::start, x1, x2};
        edges[2 * i + 1] = {y2, event_t::finish, x1, x2};
    }

    std::sort(edges.begin(), edges.end());
    std::sort(X.begin(), X.end());
    const auto it = std::unique(X.begin(), X.end());
    X.erase(it, X.end());

    const int countof_x = (int)X.size();
    std::unordered_map<int, int> order;
    for (int i = 0; i < countof_x; ++i) 
        order[X[i]] = i;
    std::vector<int> weight(countof_x - 1);
    for (int i = 0; i < countof_x - 1; ++i)
        weight[i] = X[i + 1] - X[i];

    int prev_y{-1};
    int64_t ret{0};
    cover_query cq(weight);
    for (const auto& [y, evt, x1, x2]: edges) {
        ret += int64_t(y - prev_y) * cq.query();
        cq.change(order[x1], order[x2], evt);
        prev_y = y;
    }

    return ret;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": " << solve() << "\n";
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 rectangles_union.cpp -o rectangles_union.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 rectangles_union.cpp -o rectangles_union.o

Run:
rectangles_union.exe < rectangles_union.in

Input:

3
1
0 0 5 5
3
1 1 10 2
0 0 4 4
2 2 6 6
8
1 0 3 3
2 1 5 4
1 3 3 5
1 5 2 6
3 2 4 3
2 0 7 1
5 1 6 3
5 4 7 6

Output:

Case #1: 27
Case #2: 25
Case #3: 34

*/

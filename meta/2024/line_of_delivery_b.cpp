
#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <format>
#include <random>
#include <numeric>
#include <utility>
#include <vector>

// Line of Delivery B
// https://www.facebook.com/codingcompetitions/hacker-cup/2024/practice-round/problems/D2

static std::mt19937 _gen{std::random_device{}()};

/**
 * In-order search gives sorted sequence
 * Priority decreases with depth
 * Splitting with existing value moves it to the left
 */
namespace treap
{
    struct treap_node_t
    {
        treap_node_t(int key) : _key(key) {}
        ~treap_node_t()
        {
            delete _left;
            delete _right;
        }
    
        std::uniform_int_distribution<> dist{};
        int _key;
        int _priority{dist(_gen)};
        int _size{1};
        int _lazy_delta{};
        treap_node_t* _left{};
        treap_node_t* _right{};
    };

    using pnode = std::add_pointer_t<treap_node_t>;

    std::vector<int> order;

    pnode create(int key) { return new treap_node_t(key); }

    int get_size(pnode n)
    {
        return n ? n->_size : 0;
    }

    void update_size(pnode n)
    {
        if (n)
            n->_size = 1 + get_size(n->_left) + get_size(n->_right);
    }

    void push(pnode n)
    {
        if (!n || n->_lazy_delta == 0) return;
        const int delta = std::exchange(n->_lazy_delta, 0);

        n->_key += delta;

        if (n->_left) n->_left->_lazy_delta += delta;
        if (n->_right) n->_right->_lazy_delta += delta;
    }

    void merge(pnode& t, pnode l, pnode r)
    {
        push(l);
        push(r);

        if (!l || !r)
            t = l ? l : r;
        else if (l->_priority > r->_priority)
            merge(l->_right, l->_right, r), t = l;
        else
            merge(r->_left, l, r->_left), t = r;

        update_size(t);
    }

    void split(pnode t, const int key, pnode& l, pnode& r)
    {
        if (!t) {
            l = r = nullptr;
            return;
        }

        push(t);
        if (t->_key <= key)
            split(t->_right, key, t->_right, r), l = t;
        else
            split(t->_left, key, l, t->_left), r = t;

        update_size(t);
    }

    void insert(pnode& t, pnode it)
    {
        if (!t)
            t = it;
        else if (t->_priority < it->_priority)
            split(t, it->_key, it->_left, it->_right), t = it;
        else
            insert(it->_key < t->_key ? t->_left : t->_right, it);
    }
    
    void insert(pnode& t, const int key)
    {
        insert(t, create(key));
    }

    int get_min_key(pnode t)
    {
        assert(t);
        while (t->_left) {
            push(t);
            t = t->_left;
        }

        push(t);
        return t->_key;
    }

    int get_max_key(pnode t)
    {
        assert(t);
        while (t->_right) {
            push(t);
            t = t->_right;
        }

        push(t);
        return t->_key;
    }

    void dfs_output(pnode t)
    {
        if (!t)
            return;

        push(t);
        dfs_output(t->_left);
        order.push_back(t->_key);
        dfs_output(t->_right);
    }

    const std::vector<int>& get_order(pnode t)
    {
        order.clear();
        order.reserve(t->_size);
        dfs_output(t);
        return order;
    }
}

template <typename T, typename U>
static T first_true(T lo, T hi, U f) {
    hi++;
    assert(lo <= hi); // assuming f is increasing
    while (lo < hi) { // find first index such that f is true
        const T mid = lo + (hi - lo) / 2; // this will work for negative numbers too
        f(mid) ? hi = mid : lo = mid + 1;
    }
    return lo;
}

std::vector<int> get_positions(const int N)
{
    int energy;
    treap::pnode l, r;
    treap::pnode root = nullptr;
    const auto check = [&](const int position) {
        treap::split(root, position, l, r);
        if (l == nullptr) {
            root = r;
            return true;
        }
        const bool available = treap::get_max_key(l) < position;
        int target = energy + l->_size;
        if (!available)
            --target;
        const bool result = (available && target <= position) || target < position;
        treap::merge(root, l, r);
        return result;
    };

    for (int i = 0; i < N; ++i) {
        std::cin >> energy;
        const int position = first_true(energy, energy + i, check);
        treap::split(root, position, l, r);
        if (l) {
            l->_lazy_delta -= 1;
            //treap::merge(root, l, r);
        } //else
            //root = r;

        //treap::insert(root, position);
        treap::pnode tmp{};
        treap::merge(tmp, l, treap::create(position));
        treap::merge(root, tmp, r);
    }

    const auto order = treap::get_order(root);
    delete root;
    return order;
}

void solve()
{
    int N, G;
    std::cin >> N >> G;
    const auto positions = get_positions(N);
    std::pair<int, int> best{2'000'000, -1};
    for (int i = 0; i < N; ++i) {
        const int pos = positions[i];
        const int diff = std::abs(pos - G);
        if (diff <= best.first)
            best = {diff, N - i};
    }

    std::cout << best.second << ' ' << best.first;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << std::format("Case #{}: ", g);
        solve();
        std::cout << '\n';
    }
}

/*

Format:
clang-format -i line_of_delivery_b.cpp

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 line_of_delivery_b.cpp -o line_of_delivery_b.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address line_of_delivery_b.cpp -o line_of_delivery_b

Run:
line_of_delivery_b.exe < line_of_delivery_b.in

Input:

5
2 5
7
2
3 6
9
5
7
4 8
8
7
9
6
2 10
15
5
4 4
8
5
8
5

Output:

Case #1: 1 2
Case #2: 3 4
Case #3: 3 0
Case #4: 1 5

*/

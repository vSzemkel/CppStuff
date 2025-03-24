
#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <random>
#include <numeric>
#include <utility>
#include <vector>

// Treap
// https://cp-algorithms.com/data_structures/treap.html

static std::mt19937 _gen{std::random_device{}()};

/*
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
        else {
            push(t);
            insert(it->_key < t->_key ? t->_left : t->_right, it);
        }

        update_size(t);
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

template <typename C>
static void print(const C& v, std::ostream& task_out = std::cout)
{
    if (v.empty())
        return;
    char sep = ' ';
    auto lst = v.size();
    for (const auto& e : v) {
        if (--lst == 0) sep = '\n';
        task_out << e << sep;
    }
}

int main(int, char**)
{
    std::array<int, 19> a{};
    std::iota(a.begin(), a.end(), 1);
    std::shuffle(a.begin(), a.end(), _gen);
    auto root = treap::create(0);
    for (const int v : a)
        treap::insert(root, treap::create(v));

    print(treap::get_order(root));

    treap::pnode l, r;
    treap::split(root, 10, l, r);
    assert(treap::get_max_key(l) == 10);
    assert(treap::get_min_key(r) == 11);
    r->_lazy_delta += 10;
    treap::merge(root, l, r);
    print(treap::get_order(root));

    treap::split(root, 25, l, r); // old 15 node
    r->_lazy_delta += 10;
    treap::merge(root, l, r);
    print(treap::get_order(root));
    
    treap::split(root, 7, l, r);
    l->_lazy_delta -= 10;
    treap::merge(root, l, r);
    print(treap::get_order(root));

    assert(treap::get_size(root) == treap::get_size(root->_left) + treap::get_size(root->_right) + 1);
    assert((treap::get_order(root) == std::vector{-10, -9, -8, -7, -6, -5, -4, -3, 8, 9, 10, 21, 22, 23, 24, 25, 36, 37, 38, 39}));
    delete root;
}

/*

Format:
clang-format -i treap.cpp

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 treap.cpp -o treap.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address treap.cpp -o treap

Output:

0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19
0 1 2 3 4 5 6 7 8 9 10 21 22 23 24 25 26 27 28 29
0 1 2 3 4 5 6 7 8 9 10 21 22 23 24 25 36 37 38 39
-10 -9 -8 -7 -6 -5 -4 -3 8 9 10 21 22 23 24 25 36 37 38 39

*/


#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <vector>

// Treap
// https://cp-algorithms.com/data_structures/treap.html

static std::mt19937 _gen{std::random_device{}()};

struct treap_node_t
{
    static treap_node_t* create(int key) { return new treap_node_t(key); }
    ~treap_node_t() { delete _left; delete _right; }

    std::uniform_int_distribution<> dist{};
    int _key;
    int _priority{dist(_gen)};
    int _size{1};
    int _lazy_delta{};
    treap_node_t* _left{};
    treap_node_t* _right{};

  private:
    treap_node_t(int key) : _key(key) {}
};

namespace treap
{
    using pnode = std::add_pointer_t<treap_node_t>;

    int update_size(pnode n) {
        if (n)
            return n->_size = 1 + update_size(n->_left) + update_size(n->_right);

        return 0;
    }

    void push(pnode n) {
        if (!n) return;
        const int delta = std::exchange(n->_lazy_delta, 0);
    
        n->_key += delta;

        if (n->_left) n->_left->_lazy_delta += delta;
        if (n->_right) n->_right->_lazy_delta += delta;
    }

    void merge (pnode& t, pnode l, pnode r) {
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

    void split(pnode t, const int key, pnode& l, pnode& r) {
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
}

int main(int, char**)
{
    treap_node_t* root = treap_node_t::create(0);
    treap::merge(root, root, treap_node_t::create(1));
    delete root;
    // root.describe_distribution();
}

/*

Format:
clang-format -i treap.cpp

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 treap.cpp -o treap.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address treap.cpp -o treap

Run:
treap.exe < treap.in

Input:


Output:


*/

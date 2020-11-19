
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// Friends
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ff49/000000000043aee7#analysis

class node_t
{
  public:
    node_t(const std::string& name);
    bool is_friend(const node_t& other) const { return _letters & other._letters; };
    bool _visited{false};
  private:
    uint32_t _letters{0};
};

node_t::node_t(const std::string& name)
{
    for (const char letter : name)
        _letters |= 1 << (letter - 'A');
}

class graph_t {
  public:
    void init();
    void reset();
    void solve();
  private:
    int bfs(uint32_t depth);
    int64_t hash(uint32_t n, uint32_t m);
    std::vector<uint32_t> mark_friends(uint32_t n);

    uint32_t _v, _p, _target;
    std::vector<node_t> _nodes;
    std::vector<uint32_t> _bfs_cache;
    std::unordered_map<int64_t, int> _cache;
};

int64_t graph_t::hash(uint32_t n, uint32_t m)
{
    if (n > m) std::swap(n, m);
    return ((int64_t)n << 32) + m;
}

void graph_t::reset()
{
    _bfs_cache.clear();
    for (auto& n : _nodes)
        n._visited = false;
}

void graph_t::init()
{
    std::string name;
    std::cin >> _v >> _p;
    assert(_v > 1);

    _cache.clear();
    _nodes.clear();
    for (int i = 0; i < _v; ++i) {
        std::cin >> name;
        _nodes.emplace_back(name);
    }
}

std::vector<uint32_t> graph_t::mark_friends(uint32_t n) {
    std::vector<uint32_t> ret;
    const auto& current = _nodes[n];
    for (int i = 0; i < _nodes.size(); ++i) {
        auto& node = _nodes[i];
        if (node._visited == false && node.is_friend(current)) {
            node._visited = true;
            ret.push_back(i);
        }
    }
    return ret;
}

int graph_t::bfs(const uint32_t depth)
{
    std::vector<uint32_t> next_level;
    for (const auto current : _bfs_cache) {
        if (current == _target)
            return depth;

        for (const auto n : mark_friends(current))
            next_level.push_back(n);
    }

    if (next_level.empty())
        return -1;

    _bfs_cache = next_level;
    return bfs(depth + 1);
}

void graph_t::solve() {
    int64_t start, ret;
    for (int i = 0; i < _p; ++i) {
        reset();
        std::cin >> start >> _target;
        --start; --_target;
        const int64_t h = hash(start, _target);
        auto cached = _cache.find(h);
        if (cached == _cache.end()) {
            _nodes[start]._visited = true;
            _bfs_cache.push_back(start);
            ret = bfs(1);
            _cache[h] = ret;
        } else
            ret = cached->second;

        std::cout << ret << " ";
    }
}

int main(int argc, char* argv[])
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    // parse console input
    graph_t graph;
    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        graph.init();
        std::cout << "Case #" << g << ": ";
        graph.solve();
        std::cout << "\n";
    }
}

/*
clang++.exe -Wall -ggdb3 -O0 -std=c++17 friends.cpp -o friends.exe
g++ -Wall -ggdb3 -O0 -std=c++14 friends.cpp -o friends.o
friends.exe < friends.in

Input:

2
5 2
LIZZIE KEVIN BOHDAN LALIT RUOYU
1 2
1 3
2 2
KICK START
1 2
1 2

Output:

2
5 2
LIZZIE KEVIN BOHDAN LALIT RUOYU
1 2
1 3
2 2
KICK START
1 2
1 2

*/

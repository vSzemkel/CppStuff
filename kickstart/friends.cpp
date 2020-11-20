
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// Friends
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ff49/000000000043aee7#analysis

struct node_t
{
    node_t(const std::string& name);
    int32_t _id;
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
    void solve();
  private:
    int bfs();
    uint32_t hash(uint16_t m, uint16_t n);

    std::vector<node_t> _nodes;
    uint16_t _v, _p, _start, _target;
    std::unordered_map<uint32_t, uint8_t> _cache;
};

uint32_t graph_t::hash(uint16_t m, uint16_t n)
{
    if (m > n) std::swap(m, n);
    return ((int16_t)m << 16) + n;
}

void graph_t::init()
{
    std::cin >> _v >> _p;
    assert(_v > 1);

    _cache.clear();
    _nodes.clear();
    std::string name;
    for (int i = 0; i < _v; ++i) {
        std::cin >> name;
        _nodes.emplace_back(name)._id = i;
    }
}

int graph_t::bfs()
{
    auto v = _v;
    auto nodes = _nodes;

    auto& start = nodes[_start];
    uint16_t depth{2};
    uint32_t hull{start._letters};
    start._letters = 0;
    while (true) {
        const auto pp = std::partition(nodes.begin(), nodes.begin() + v, [](const node_t& n){ return n._letters > 0; });
        v = pp - nodes.begin();

        uint32_t next_hull{0};
        for (int i = 0; i < v; ++i) {
            auto& n = nodes[i];
            if (n._letters & hull) {
                if (depth <= std::numeric_limits<uint8_t>::max())
                    _cache[hash(_start, n._id)] = depth;

                if (n._id == _target)
                    return depth;

                next_hull |= n._letters;
                n._letters = 0;
            }
        }

        if (next_hull == 0)
            return -1;

        hull = next_hull;
        ++depth;
    }

    return depth;
}

void graph_t::solve() {
    for (int i = 0; i < _p; ++i) {
        std::cin >> _start >> _target;
        --_start; --_target;

        const int64_t h = hash(_start, _target);
        auto cached = _cache.find(h);
        const auto ret = (cached == _cache.end()) ? bfs() : cached->second;
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

3
5 2
LIZZIE KEVIN BOHDAN LALIT RUOYU
1 2
1 3
2 2
KICK START
1 2
1 2
16 3
AB BC CD DE EF AG AH AI AJ CJ BX YS KT OP FU CW
1 5
5 1
2 4

Output:

Case #1: 2 3 
Case #2: -1 -1
Case #3: 5 5 3

*/

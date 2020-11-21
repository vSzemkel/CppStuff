
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// Friends
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ff49/000000000043aee7#analysis

struct node_t
{
    node_t(const std::string& name);
    int32_t _letters{0};
    int32_t _group{0};
    uint16_t _id;
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
    void group();
    uint32_t hash(uint16_t m, uint16_t n) const;

    std::vector<node_t> _nodes;
    uint16_t _v, _p, _start, _target;
    std::unordered_map<uint32_t, int8_t> _cache;
};

uint32_t graph_t::hash(const uint16_t m, const uint16_t n) const
{
    return _nodes[m]._letters ^ _nodes[n]._letters;
}

void graph_t::group()
{
    for (auto& n : _nodes)
        if (n._group == 0) {
            n._group = -1;
            int32_t group_total = n._letters;
            std::vector<int32_t> group_dfs = { n._id };
            std::vector<int32_t> group_members = { n._id };
            while (!group_dfs.empty()) {
                group_dfs.pop_back();
                for (auto& n2 : _nodes) {
                    if (n2._group == 0 && (n2._letters & group_total)) {
                        group_total |= n2._letters;
                        n2._group = -1;
                        group_dfs.push_back(n2._id);
                        group_members.push_back(n2._id);
                    }
                }
            }
            for (auto& m : group_members)
                _nodes[m]._group = group_total;
        }
}

void graph_t::init()
{
    std::cin >> _v >> _p;
    assert(_v > 1);

    _cache.clear();
    _nodes.clear();
    std::string name;
    for (uint16_t i = 0; i < _v; ++i) {
        std::cin >> name;
        _nodes.emplace_back(name)._id = i;
    }
}

int graph_t::bfs()
{
    if (_nodes[_start]._group != _nodes[_target]._group)
        return -1;
    if (_nodes[_start]._letters & _nodes[_target]._letters)
        return 2;

    const auto search_hash = hash(_start, _target);
    const auto cached = _cache.find(search_hash);
    if (cached != _cache.end())
        return cached->second;

    std::vector<node_t> nodes;
    std::copy_if(_nodes.begin(), _nodes.end(), std::back_inserter(nodes), [g = _nodes[_start]._group](const node_t& n){ return n._group == g; });
    uint16_t v = nodes.size();

    auto& start = *std::find_if(nodes.begin(), nodes.begin() + v, [s = _start](const node_t& n){ return n._id == s; });
    int8_t depth{2};
    int32_t hull{start._letters};
    start._letters = 0;

    while (true) {
        const auto pp = std::partition(nodes.begin(), nodes.begin() + v, [](const node_t& n){ return n._letters > 0; });
        v = pp - nodes.begin();

        uint32_t next_hull{0};
        for (uint16_t i = 0; i < v; ++i) {
            auto& n = nodes[i];
            if (n._letters & hull) {
                if (depth > 2) {
                    const auto node_hash = hash(_start, n._id);
                    const auto cached = _cache.find(node_hash);
                    if (cached == _cache.end())
                        _cache[node_hash] = depth;
                }

                if (n._id == _target)
                    return depth;

                next_hull |= n._letters;
                n._letters = 0;
            }
        }

        hull = next_hull;
        ++depth;
    }
}

void graph_t::solve() {
    init();  // read data, build graph
    group(); // identify connected regions

    for (uint16_t i = 0; i < _p; ++i) {
        std::cin >> _start >> _target;
        --_start; --_target;
        std::cout << " " << bfs();
    }
}

void smart() {
    using vec = std::vector<int>;
    constexpr size_t letters = 26;
    constexpr int bad = letters + 1;
    std::vector<vec> costs(letters, vec(letters, bad));

    uint16_t v, p, start, target;
    std::cin >> v >> p;
    std::string name;
    std::vector<std::string> names(v);
    for (auto& name : names) {
        std::cin >> name;
        for (int j = 0; j < name.size() - 1; ++j)
            for (int k = j + 1; k < name.size(); ++k)
                costs[name[j] - 'A'][name[k] - 'A'] = costs[name[k] - 'A'][name[j] - 'A'] = 1;
    }

    for (int i = 0; i < letters ; ++i)
        costs[i][i] = 0;

    for (int k = 0; k < letters; ++k)
        for (int i = 0; i < letters - 1; ++i)
            for (int j = i + 1; j < letters; ++j)
                costs[i][j] = costs[j][i] = std::min(costs[i][j], costs[i][k] + costs[k][j]);

    for (int i = 0; i < p; ++i) {
        std::cin >> start >> target;
        --start; --target;
        int ca{bad};
        for (const char s : names[start])
            for (const char t : names[target])
                ca = std::min(ca, costs[s - 'A'][t - 'A']);
        ca = ca < bad ? ca + 2 : -1;
        std::cout << " " << (int)ca;
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
        //std::cout << "Case #" << g << ":"; graph.solve(); std::cout << "\n";
        std::cout << "Case #" << g << ":"; smart(); std::cout << "\n";
    }
}

/*
clang++.exe -Wall -ggdb3 -O0 -std=c++17 friends.cpp -o friends.exe
g++ -Wall -ggdb3 -O0 -std=c++17 friends.cpp -o friends.o
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
18 4
AB BC CD DE EF AG AH AI AJ CJ BX YS KT OP FU CW VY VZ
1 5
5 1
2 4
12 18

Output:

Case #1: 2 3 
Case #2: -1 -1 
Case #3: 5 5 3 3 

*/

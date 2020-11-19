
#include <algorithm>
#include <array>
#include <assert.h>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <filesystem>
#include <fstream>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <random>
#include <set>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// friends
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ff49/000000000043aee7#analysis

class node_t
{
  public:
    node_t(std::string name);
    bool has_letter(char l) { return std::binary_search(_sorted_name.begin(), _sorted_name.end(), l); };
    std::string _name;
  private:
    std::string _sorted_name;
};

node_t::node_t(std::string name) : _name(name), _sorted_name(name)
{
    std::sort(_sorted_name.begin(), _sorted_name.end());
}

class graph_t {
  public:
    void init();
    void solve();
  private:
    uint32_t _v;
    std::vector<node_t> _nodes;
    std::vector<std::pair<uint32_t, uint32_t>> _pairs;
};

void graph_t::init()
{
    uint32_t pairs, f, s;
    std::string name;
    std::cin >> _v >> pairs;
    for (int i = 0; i < _v; ++i) {
        std::cin >> name;
        _nodes.emplace_back(name);
    }
    for (int i = 0; i < pairs; ++i) {
        std::cin >> f >> s;
        _pairs.emplace_back(f - 1, s - 1);
    }
}

void graph_t::solve() {
    return;
}

graph_t g_graph;

int main(int argc, char* argv[])
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    // parse console input
    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        g_graph.init();
        std::cout << "Case #" << g << ": ";
        g_graph.solve();
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


*/

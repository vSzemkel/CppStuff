
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>

// Stable wall
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ff43/00000000003379bb

std::vector<int> stack;
std::vector<bool> visited;
std::vector<std::vector<int>> graph;

static void dfs(const int n) {
    visited[n] = true;
    for (const int ch : graph[n])
        if (!visited[ch])
            dfs(ch);
    stack.push_back(n);
}

static void solve() {
    int R, C; std::cin >> R >> C;
    std::vector<std::string> wall(R);
    for (auto& r : wall) {
        std::cin >> r;
        assert(int(r.size()) == C);
    }

    std::map<char, std::unordered_set<char>> tiles;
    for (int c = 0; c < C; ++c) {
        std::vector<char> store(1, '$');
        for (int r = 0; r < R; ++r) {
            const char p = wall[r][c];
            if (store.back() == p)
                continue;
            if (std::find(store.begin() + 1, store.end(), p) != store.end()) {
                std::cout << -1;
                return;
            }
            for (const auto t : store)
                tiles[t].insert(p);
            store.push_back(p);
            if (tiles.find(p) == tiles.end())
                tiles[p] = {};
        }
    }
    tiles.erase('$');

    std::vector<char> char_map;
    std::map<char, int> rev_char_map;
    for (const auto& t : tiles) {
        char_map.push_back(t.first);
        rev_char_map[t.first] = char_map.size() - 1;
    }

    const auto size = int(tiles.size());
    graph.assign(size, {});
    for (const auto& t : tiles)
        for (const auto n : t.second)
            graph[rev_char_map[t.first]].push_back(rev_char_map[n]);

    stack.clear();
    stack.reserve(size);
    visited.assign(size, false);
    for (int i = 0; i < size; ++i) 
        if (!visited[i])
            dfs(i);

    for (auto it = stack.begin(); it != stack.end(); ++it)
        std::cout << char_map[*it];
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 stable_wall.cpp -o stable_wall.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address stable_wall.cpp -o stable_wall

Run:
stable_wall.exe < stable_wall.in

Input:

4
4 6
ZOAAMM
ZOAOMM
ZOOOOM
ZZZZOM
4 4
XXOO
XFFO
XFXO
XXXO
5 3
XXX
XPX
XXX
XJX
XXX
3 10
AAABBCCDDE
AABBCCDDEE
AABBCCDDEE

Output:

Case #1: ZOAM
Case #2: -1
Case #3: -1
Case #4: EDCBA

*/

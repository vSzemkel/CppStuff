/*
ID: marcin.8
LANG: C++
TASK: race3
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=c7zTMGpJrkw&S=race3
*/

#include <algorithm>
#include <array>
#include <assert.h>
#include <bitset>
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
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <stdlib.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <vector>

std::ifstream task_in("race3.in");
std::ofstream task_out("race3.out");

struct digraph_t
{
    digraph_t(const int size = 0) : _size(size), _order(size), _in(size), _out(size) {
    }

    void add_edge(const int from, const int to) {
        assert(0 <= from && from < _size);
        assert(0 <= to && to < _size);
        _out[from].push_back(to);
        _in[to].push_back(from);
    }

    auto get_sccomponents() {
        _order.clear();
        _used.assign(_size, false);
        for (int i = 0; i < _size; ++i) {
            if (!_used[i])
                first_dfs(i);
        }

        std::swap(_in, _out);
        _comp.assign(_size, -1);
        for (int i = 0, ci = 0; i < _size; ++i) {
            const int v = _order[_size - i - 1];
            if (_comp[v] == -1)
                second_dfs(v, ci++);
        }
        std::swap(_in, _out);

        return _comp;
    }

  private:
    int _size;
    std::vector<bool> _used;
    std::vector<int> _comp, _low, _order;
    std::vector<std::vector<int>> _in, _out;

    void first_dfs(const int v) {
        _used[v] = true;
        for (int u : _out[v]) {
            if (!_used[u])
                first_dfs(u);
        }

        _order.push_back(v);
    }

    void second_dfs(const int v, const int ci) {
        _comp[v] = ci;
        for (int u : _out[v]) {
            if (_comp[u] == -1)
                second_dfs(u, ci);
        }
    }
};

constexpr const int INF = 1e09;

int size;
std::vector<bool> seen;
std::vector<int> dist, count, pred, scc;
std::vector<std::vector<int>> input(1);

static void shortest_paths(int source = 0)
{
    std::vector<bool> in_queue(size);
    std::queue<int> q;
    q.push(source);
    dist.assign(size, INF);
    dist[source] = 0;

    while (!q.empty()) {
        const int cur = q.front(); q.pop();
        in_queue[cur] = false;
        for (const auto& next : input[cur])
            if (dist[cur] + 1 < dist[next]) {
                dist[next] = dist[cur] + 1;
                if (!in_queue[next]) {
                    q.push(next);
                    in_queue[next] = true;
                }
            }
    }
}

int main(int, char**)
{
    while (true) {
        int n; task_in >> n;
        if (n == -1) break;
        if (n == -2)
            input.push_back({});
        else
            input.back().push_back(n);
    }

    size = int(input.size()) - 1;
    digraph_t g(size);
    for (int f = 0; f < size; ++f)
        for (const int t : input[f])
            g.add_edge(f, t);
    scc = g.get_sccomponents();
    std::unordered_map<int, int> sccs;
    for (int i = 0; i < int(scc.size()); ++i)
        ++sccs[scc[i]];

    // if unavoidable vertex has no out edges, target is unreachable
    std::vector<int> unavo, split;
    for (int i = 1; i < size - 1; ++i) {
        auto tmp = std::move(input[i]);
        input[i].clear();
        shortest_paths();
        if (dist[size - 1] == INF)
            unavo.push_back(i);
        input[i] = std::move(tmp);
    }

    // only unavoidable vertexes that are begining of SCC are the splitting ones
    for (const int i : unavo)
        if (sccs[scc[i]] == 1 || i == *std::find(scc.begin(), scc.end(), scc[i]))
            split.push_back(i);

    task_out << unavo.size();
    for (const int u : unavo) task_out << ' ' << u;
    task_out << '\n' << split.size();
    for (const int s : split) task_out << ' ' << s;
    task_out << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 race3.cpp -o race3.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address race3.cpp -o race3

Input:

1 2 -2
3 -2
3 -2
5 4 -2
6 4 -2
6 -2
7 8 -2
9 -2
5 9 -2
-2
-1

Output:

2 3 6
1 3

*/

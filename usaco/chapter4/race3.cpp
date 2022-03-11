/*
ID: marcin.8
LANG: C++
TASK: race3
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=c7zTMGpJrkw&S=race3
*/

#include <algorithm>
#include <assert.h>
#include <fstream>
#include <queue>
#include <unordered_map>
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

    auto get_unavoidables(const int src, const int snk) {
        _unavo.clear();
        if (!reachable(src, snk))
            return _unavo;
        for (int n = 0; n < _size; ++n) {
            if (n == src || n == snk)
                continue;
            auto tmp = std::move(_out[n]);
            _out[n].clear();
            if (!reachable(src, snk))
                _unavo.push_back(n);
            _out[n] = std::move(tmp);
        }

        return _unavo;
    }

  private:
    int _size;
    std::vector<bool> _used;
    std::vector<int> _comp, _low, _order, _unavo;
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

    bool reachable(const int src, const int snk) {
        _used.assign(_size, false);
        first_dfs(src);
        return _used[snk];
    }
};

int main(int, char**)
{
    std::vector<std::vector<int>> input(1);
    while (true) {
        int n; task_in >> n;
        if (n == -1) break;
        if (n == -2)
            input.push_back({});
        else
            input.back().push_back(n);
    }

    int size = int(input.size()) - 1;
    digraph_t g(size);
    for (int f = 0; f < size; ++f)
        for (const int t : input[f])
            g.add_edge(f, t);
    const auto scc = g.get_sccomponents();
    std::unordered_map<int, int> sccs;
    for (int i = 0; i < int(scc.size()); ++i)
        ++sccs[scc[i]];

    // if unavoidable vertex has no out edges, target is unreachable
    const auto unavo = g.get_unavoidables(0, size - 1);

    // only unavoidable vertexes that are begining of SCC are the splitting ones
    std::vector<int> split;
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

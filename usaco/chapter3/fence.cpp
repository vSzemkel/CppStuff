/*
ID: marcin.8
LANG: C++
TASK: fence
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=rJ1Z3Rf9Suy&S=fence
*/

#include <algorithm>
#include <assert.h>
#include <fstream>
#include <map>
#include <vector>

std::ifstream task_in("fence.in");
std::ofstream task_out("fence.out");

template <typename T>
struct euler_graph_t {
    struct edge_t {
        int dest;
        bool present;
    };

    void add_edge(const T from, const T to) {
        int f{-1}, t{-1};
        auto it = _index.find(from);
        if (it == _index.end()) {
            _index[from] = _size;
            _adj.emplace_back();
            _label.push_back(from);
            f = _size++;
        } else
            f = it->second;

        it = _index.find(to);
        if (it == _index.end()) {
            _index[to] = _size;
            _adj.emplace_back();
            _label.push_back(to);
            t = _size++;
        } else
            t = it->second;

        assert(0 <= f && f < _size && 0 <= t && t < _size);

        _adj[f].push_back(_num_edges++);
        _edges.push_back({t, true});
        _adj[t].push_back(_num_edges++);
        _edges.push_back({f, true});
    }

    /**
     * @brief Finds one with minimal label of starting points of an Euler path
     * 
     * @return int Index in _adj or -1
     */
    int check_euler() {
        int pos{0}, odd_nodes{0}, min_all{0}, min_odd{-1};
        for (const auto& n : _adj) {
            if (!std::all_of(n.begin(), n.end(), [this](const int e){ return _edges[e].present; }))
                return -1;
            if (n.size() & 1) {
                ++odd_nodes;
                if (odd_nodes == 1)
                    min_odd = pos;
                else if (_label[pos] < _label[min_odd])
                    min_odd = pos;
            }
            if (_label[pos] < _label[min_all])
                min_all = pos;
            ++pos;
        }

        if (odd_nodes != 0 && odd_nodes != 2)
            return -1;

        return ~min_odd ? min_odd : min_all;
    }

    auto euler_path(const int source = 0) {
        std::vector<T> path;
        std::vector<int> stack(1, source);
        while (!stack.empty()) {
            const int node = stack.back();
            auto& neibors = _adj[node];
            //const auto next = std::find_if(neibors.begin(), neibors.end(), [this](const auto& e){ return _edges[e].present; });
            const auto next = *std::min_element(neibors.begin(), neibors.end(), [this](const auto& e1, const auto& e2){ 
                return _edges[e1].present > _edges[e2].present
                   || (_edges[e1].present == _edges[e2].present && _label[_edges[e1].dest] < _label[_edges[e2].dest]);
            });
            if (!_edges[next].present) {
                stack.pop_back();
                path.push_back(_label[node]);
            } else {
                _edges[next].present = false;
                _edges[next ^ 1].present = false;
                stack.push_back(_edges[next].dest);
            }
        }

        return path;
    }

  private:
    int _size{0}, _num_edges{0};
    std::vector<T> _label;
    std::map<T, int> _index; // unordered will not work with unhashable T
    std::vector<edge_t> _edges;
    std::vector<std::vector<int>> _adj; // _adj[i][j] stores an index to _edges
};

int main(int, char**)
{
    int N; task_in >> N;
    euler_graph_t<int> eg;
    for (int z = N; z; --z) {
        int from, to; 
        task_in >> from >> to;
        eg.add_edge(from, to);
    }

    const int start = eg.check_euler();
    assert(start >= 0);
    auto path = eg.euler_path(start);
    std::reverse(path.begin(), path.end());
    for (const auto node : path)
        task_out << node << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 fence.cpp -o fence.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address fence.cpp -o fence

Input:

15 
2 9 
9 7 
9 5 
5 10 
10 1 
1 7 
7 10 
7 3 
3 4 
4 5 
5 3 
3 6 
6 5 
4 10 
5 2

Output:

4
3
5
2
9
5
4
10
1
7
3
6
5
10
7
9

*/

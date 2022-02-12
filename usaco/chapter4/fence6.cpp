/*
ID: marcin.8
LANG: C++
TASK: fence6
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=b4uVVw7TtuR&S=fence6
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

std::ifstream task_in("fence6.in");
std::ofstream task_out("fence6.out");

constexpr const int INF = 1e06;

template <typename T = int>
struct graph_t
{
    using edge_t = std::array<int, 2>; // {to, cost}

    graph_t(const int size = 0) : _size(size), _adj(size) {}

    void add_edge(const int from, const int to, const int cost = 0) {
        assert(0 <= from && from < _size && 0 <= to && to < _size);
        _adj[from].push_back({to, cost});
        _adj[to].push_back({from, cost});
    }

    bool has_edge(const int from, const int to) const {
        const auto& c = _adj[from];
        return std::find_if(c.begin(), c.end(), [to](const auto& e){ return e[0] == to; }) != c.end();
    }

    void reset() {
        _ccsz = _time = 0;
        _has_neg_cycle = false;
        _cc.assign(_size, -1);
        _low.assign(_size, -1);
        _pred.assign(_size, -1);
        _order.assign(_size, -1);
        _dist.assign(_size, INF);
        _seen.assign(_size, false);
        std::iota(_pred.begin(), _pred.end(), 0);
    }

    auto floyd_warshall() { // compute all distances
        std::vector<std::vector<int>> dist(_size, std::vector<int>(_size, INF));
        for (int i = 0; i < _size; ++i) {
            dist[i][i] = 0;
            for (const auto& e : _adj[i])
                dist[i][e[0]] = e[1];
        }

        for (int k = 0; k < _size; ++k)
            for (int i = 0; i < _size; ++i)
                for (int j = 0; j < _size; ++j)
                    if (i != j)
                        dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);

        return dist;
    }

  private:
    int _size, _ccsz, _time;
    bool _has_neg_cycle;
    std::vector<T> _label;
    std::vector<bool> _seen;
    std::vector<int> _cc, _low; // concomp index, low table for find_bridges
    std::vector<int> _dist, _pred, _order;
    std::map<T, int> _index; // unordered will not work with unhashable T
    std::vector<std::vector<edge_t>> _adj;
};

int main(int, char**)
{
    int N; task_in >> N;
    std::vector<int> length(N); // {edge -> length}
    std::vector<std::pair<std::vector<int>, std::vector<int>>> endpoints(N); // { edge -> in, out }
    for (int i = 0; i < N; ++i) {
        int lab, in, out;
        task_in >> lab >> length[i] >> in >> out;
        --lab;
        auto& ine = endpoints[lab].first;
        ine.resize(in);
        for (auto& e : ine) {
            task_in >> e; --e;
        }
        auto& oute = endpoints[lab].second;
        oute.resize(out);
        for (auto& e : oute) {
            task_in >> e; --e;
        }
        if (in > out)
            std::swap(endpoints[lab].first, endpoints[lab].second);
    }

    // remove leaf (half-free) edges
    bool repeat{true};
    while (repeat) {
        repeat = false;
        const auto it = std::find_if(endpoints.begin(), endpoints.end(), [](const auto& e){
            return e.first.empty() ^ e.second.empty();
        });
        if (it != endpoints.end()) {
            const int fence_id = it - endpoints.begin();
            auto& neib = it->second; // assert(it->first.empty())
            for (const int n : neib)
                for (auto al : { &endpoints[n].first, &endpoints[n].second })
                    for (auto it = al->begin(); it != al->end(); )
                        if (*it == fence_id)
                            it = al->erase(it);
                        else
                            ++it;
            neib.clear();
            repeat = true;
        }
    }

    graph_t<int> pastures(N);
    pastures.reset();
    for (int f = 0; f < N; ++f) {
        for (const int t : endpoints[f].first)
            if (f < t)
                pastures.add_edge(f, t, length[f] + length[t]);
        for (const int t : endpoints[f].second)
            if (f < t)
                pastures.add_edge(f, t, length[f] + length[t]);
    }

    int ans = INF;
    const auto fw = pastures.floyd_warshall();
    for (int k = 0; k < N; ++k)
        for (int i = k + 1; i < N; ++i)
            for (int j = i + 1; j < N; ++j) {
                if (pastures.has_edge(k, i) && pastures.has_edge(i, j) && pastures.has_edge(j, k)) {
                    int x{0};
                    const auto& c = endpoints[k].first;
                    if (std::find(c.begin(), c.end(), i) != c.end())
                        ++x;
                    if (std::find(c.begin(), c.end(), j) != c.end())
                        ++x;
                    if ((x & 1) == 0) // Y config, not O config
                        continue;
                }

                const auto a = fw[k][i];
                const auto b = fw[i][j];
                const auto c = fw[j][k];
                if (a < b + c && b < a + c && c < a + b) {
                    const auto can = a + b + c;
                    ans = std::min(ans, can);
                }
            }

    task_out << ans / 2 << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 fence6.cpp -o fence6.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address fence6.cpp -o fence6

Input:

10
1 16 2 2
2 7
10 6
2 3 2 2
1 7
8 3
3 3 2 1
8 2
4
4 8 1 3
3
9 10 5
5 8 3 1
9 10 4
6
6 6 1 2 
5 
1 10
7 5 2 2 
1 2
8 9
8 4 2 2
2 3
7 9
9 5 2 3
7 8
4 5 10
10 10 2 3
1 6
4 9 5

Output:

12

*/

/*
ID: marcin.8
LANG: C++
TASK: fence6
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=b4uVVw7TtuR&S=fence6
*/

#include <algorithm>
#include <array>
#include <assert.h>
#include <fstream>
#include <memory>
#include <numeric>
#include <queue>
#include <utility>
#include <vector>

std::ifstream task_in("fence6.in");
std::ofstream task_out("fence6.out");

// Hint, not used below, from the appreciated solution:
// A node is uniquely identified by two smallest edges

constexpr const int INF = 1e06;

template <typename T = int>
struct graph_t
{
    using edge_t = std::array<int, 2>; // {to, cost}

    graph_t(const int size = 0) : _size(size), _adj(size) { reset(); }

    void add_edge(const int from, const int to, const int cost = 0) {
        assert(0 <= from && from < _size && 0 <= to && to < _size);
        _adj[from].push_back({to, cost});
        _adj[to].push_back({from, cost});
    }

    void reset() {
        _pred.assign(_size, -1);
        _dist.assign(_size, INF);
        _seen.assign(_size, false);
        std::iota(_pred.begin(), _pred.end(), 0);
    }

    auto shortest_paths(const int source = 0) {
        std::queue<int> qq;
        qq.push(source); // {distance from source}
        _dist[source] = 0;
        _pred[source] = -1;
        while (!qq.empty()) {
            const int cur = qq.front(); qq.pop();
            _seen[cur] = false;
            for (const auto& [next, cost] : _adj[cur])
                if (_dist[cur] + cost < _dist[next]) {
                    _pred[next] = cur;
                    _dist[next] = _dist[cur] + cost;
                    if (!_seen[next]) {
                        qq.push(next);
                        _seen[next] = true;
                    }
                }
        }

        return _pred;
    }

    int first_path_node(const std::vector<int>& pred, const int source, int target) const {
        while (~pred[target] && pred[target] != source)
            target = pred[target];
        return target;
    }

    auto floyd_warshall_1d() { // compute all distances
        _fw_uptr = std::make_unique<int[]>(_size * _size);
        auto fw = _fw_uptr.get();
        for (int i = 0; i < _size; ++i) {
            for (int j = 0; j < _size; ++j)
                fw[i  * _size + j] = INF;
            fw[i  * _size + i] = 0;
            for (const auto& e : _adj[i])
                fw[i  * _size + e[0]] = e[1];
        }

        for (int k = 0; k < _size; ++k)
            for (int i = 0; i < _size; ++i)
                for (int j = 0; j < _size; ++j)
                    fw[i  * _size + j] = std::min(fw[i  * _size + j], fw[i  * _size + k] + fw[k  * _size + j]);

        return fw;
    }

  private:
    int _size;
    std::vector<bool> _seen;
    std::vector<int> _pred, _dist;
    std::unique_ptr<int[]> _fw_uptr;
    std::vector<std::vector<edge_t>> _adj;
};

int main(int, char**)
{
    int N; task_in >> N;
    // every fence is equivalent to node i posts graph
    // every cycle in posts graph has its perimeter doubled
    graph_t<int> posts(N);
    std::vector<int> tmp, length(N); // {edge -> length}
    std::vector<std::vector<int>> endpoints(N); // { edge -> in, out }
    for (int z = N; z; --z) {
        int b, len, in, out;
        task_in >> b >> len >> in >> out;
        --b;
        length[b] = len;
        auto& first = endpoints[b];
        auto& second = tmp;
        if (in > out)
            std::swap(first, second);
        first.resize(in);
        for (auto& e : first) {
            task_in >> e;
            if (length[--e] > 0)
                posts.add_edge(b, e, len + length[e]);
        }
        second.resize(out);
        for (auto& e : second) {
            task_in >> e;
            if (length[--e] > 0)
                posts.add_edge(b, e, len + length[e]);
        }
    }

    // precompute all source predecors 
    std::vector<std::vector<int>> pred(N);
    for (int i = 0; i < N; ++i) {
        posts.reset();
        pred[i] = posts.shortest_paths(i);
    }

    const auto has_y_config = [&](const int s, const int t1, const int t2) -> bool {
        int x{0};
        const auto p1 = posts.first_path_node(pred[s], s, t1);
        const auto p2 = posts.first_path_node(pred[s], s, t2);
        if (p1 == p2)
            return true;
        for (const int n : endpoints[s])
            if (n == p1 || n == p2)
                ++x;
        return (x & 1) == 0; // Y config, not O config
    };

    int ans = INF;
    const auto fw = posts.floyd_warshall_1d();
    for (int k = 0; k < N; ++k)
        for (int i = k + 1; i < N; ++i)
            for (int j = i + 1; j < N; ++j) {
                // with the below condition commented out partial 
                // solution may be incorrect, but the final is correct.
                const auto can = fw[k * N + i] + fw[i * N + j] + fw[j * N + k];
                if (can < ans && !has_y_config(i, j, k) && !has_y_config(j, k, i) /*&& !has_y_config(k, i, j)*/) 
                    ans = can;
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


#include <array>
#include <cassert>
#include <iostream>
#include <limits>
#include <numeric>
#include <map>
#include <queue>

// Undirected graph with basic operations
// Templated for node label datatype

template <typename T = int>
struct graph_t
{
    using edge_t = std::array<int, 2>; // {to, cost}

    graph_t(const int size = 0) : _size(size), _adj(size) {}
    graph_t(const std::vector<T>& labels) : _size(labels.size()), _labels(labels), _adj(size) {}

    int add_node(T label) {
        assert(_index.count(label) == 0);
        const int ret = _size++;
        _index[label] = ret;
        _label.push_back(std::move(label));
        _adj.push_back({});
        return ret;
    }

    void add_edge(const T& from, const T& to, const int cost = 0) {
        assert(_index.count(from) && _index.count(to));
        add_edge(_index[from], _index[to], cost);
    }

    void add_edge(const int from, const int to, const int cost = 0) {
        assert(0 <= from < _size && 0 <= to < _size);
        _adj[from].push_back({to, cost});
        _adj[to].push_back({from, cost});
    }

    void reset() {
        _pred.assign(_size, -1);
        _dist.assign(_size, INF);
        _seen.assign(_size, false);
        std::iota(_pred.begin(), _pred.end(), 0);
    }

    void clear() {
        _size = 0;
        _index.clear();
        _label.clear();
        reset();
    }

    auto get_path_to_label(const T& label) {
        assert(_index.count(label));
        const auto path = get_path_to(_index[label]);
        std::vector<T> lp;
        lp.reserve(path.size());
        std::transform(path.begin(), path.end(), std::back_inserter(lp), [&](const int i){ return _label[i]; });
        return lp;
    }

    auto get_path_to(int node) const {
        assert(0 <= node && node < _size);
        std::vector<int> rp;
        do {
            rp.push_back(node);
            node = _pred[node];
        } while (node >= 0);
        std::reverse(rp.begin(), rp.end());
        return rp;
    }

    int get_cost_to_label(const T& label) {
        assert(_index.count(label));
        return get_cost_to(_index[label]);
    }

    int get_cost_to(const int node) {
        assert(0 <= node && node < _size);
        return _dist[node];
    }

    bool found(const T& label) {
        assert(_index.count(label));
        return found(_index[label]);
    }

    bool found(const int node) {
        assert(0 <= node && node < _size);
        return _pred[node] != node;
    }

    bool check_maze() { // requires all 0 or 1 costs
        for (const auto& n : _adj)
            if (!std::all_of(n.begin(), n.end(), [](const edge_t& e){ return e[1] == 0 || e[1] == 1; }))
                return false;
        return true;
    }

    void maze(const T& from, const T& to) {
        assert(_index.count(from) && _index.count(to));
        maze(_index[from], _index[to]);
    }

    void maze(const int source = 0, const int target = -1) {
        std::deque<int> dq;
        dq.push_back(source); // {distance from source, to}
        _dist[source] = 0;
        _pred[source] = -1;
        while (!dq.empty()) {
            const auto node = dq.front(); dq.pop_front();
            if (!_seen[node]) {
                if (node == target)
                    return;
                _seen[node] = true;
                for (const auto& e : _adj[node]) {
                    const auto next = e[0];
                    const auto cand = _dist[node] + e[1];
                    if (!_seen[next] && cand < _dist[next]) {
                        _dist[next] = cand;
                        _pred[next] = node;
                        if (e[1] == 0)
                            dq.push_front(next);
                        else
                            dq.push_back(next);
                    }
                }
            }
        }
    }

    bool check_dijkstra() { // requires non negative costs
        for (const auto& n : _adj)
            if (!std::all_of(n.begin(), n.end(), [](const edge_t& e){ return e[1] >= 0; }))
                return false;
        return true;
    }

    void dijkstra(const T& from, const T& to) {
        assert(_index.count(from) && _index.count(to));
        dijkstra(_index[from], _index[to]);
    }

    void dijkstra(const int source = 0, const int target = -1) {
        std::priority_queue<edge_t, std::vector<edge_t>, std::greater<>> pq;
        pq.push({0, source}); // {distance from source, to}
        _dist[source] = 0;
        _pred[source] = -1;
        while (!pq.empty()) {
            const auto node = pq.top()[1]; pq.pop();
            if (!_seen[node]) {
                if (node == target)
                    return;
                _seen[node] = true;
                for (const auto& e : _adj[node]) {
                    const auto next = e[0];
                    const auto cand = _dist[node] + e[1];
                    if (cand < _dist[next]) {
                        _dist[next] = cand;
                        _pred[next] = node;
                        pq.push({cand, next});
                    }
                }
            }
        }
    }

    static constexpr const int INF = 1 << 30;

  private:
    int _size;
    std::vector<T> _label;
    std::vector<bool> _seen;
    std::vector<int> _pred, _dist;
    std::map<T, int> _index; // unordered will not work with unhashable T
    std::vector<std::vector<edge_t>> _adj;
};

graph_t<char> g;

void init_maze()
{
    g.clear();
    for (const char c : "ABCD")
        if (c) g.add_node(c);

    g.add_edge('A', 'B', 1);
    g.add_edge('B', 'C', 0);
    g.add_edge('C', 'D', 0);
    g.add_edge('B', 'D', 1);
    g.reset();
}

void init_dijkstra()
{
    g.clear();
    for (const char c : "ABCDEFGHIJKLM")
        if (c) g.add_node(c);

    g.add_edge('A', 'B', 3);
    g.add_edge('A', 'C', 1);
    g.add_edge('B', 'E', 2);
    g.add_edge('B', 'F', 1);
    g.add_edge('C', 'F', 2);
    g.add_edge('C', 'G', 1);
    g.add_edge('D', 'H', 1);
    g.add_edge('C', 'D', 2);
    g.add_edge('D', 'I', 2);
    g.add_edge('G', 'H', 3);
    g.add_edge('H', 'I', 1);
    g.add_edge('E', 'J', 2);
    g.add_edge('F', 'J', 3);
    g.add_edge('G', 'K', 1);
    g.add_edge('H', 'L', 1);
    g.add_edge('K', 'L', 4);
    g.add_edge('J', 'M', 1);
    g.add_edge('K', 'M', 2);
    g.reset();
}

int main(int argc, char* argv[])
{
    init_maze();
    if (g.check_maze())
        g.maze('A', 'D');
    assert((g.get_path_to_label('D') == std::vector{'A', 'B', 'C', 'D'}) && g.get_cost_to_label('D') == 1);

    init_dijkstra();
    if (g.check_dijkstra())
        g.dijkstra('A', 'M');

    // present result
    if (!g.found('M'))
        printf("\nPath not found\n");
    else {
        const auto path = g.get_path_to_label('M');
        const auto length = int(path.size());
        printf("\nFound shortest path from A to M with the length of %i and cost %i\n", length, g.get_cost_to_label('M'));
        printf("\t%c", path.front());
        for (int i = 1; i < length; ++i)
            printf("->%c", path[i]);
    }
}

/*    clang++.exe -Wall -g -std=c++17 graph.cpp -o graph.exe

Output:

Found shortest path from A to M with the length of 5 and cost 5
	A->C->G->K->M

*/
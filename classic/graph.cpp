
#include <array>
#include <cassert>
#include <iostream>
#include <limits>
#include <memory>
#include <numeric>
#include <map>
#include <queue>

// Undirected graph with basic operations
// Templated for node label datatype

template <typename T = int>
struct graph_t
{
    using edge_t = std::array<int, 2>; // {to, cost}

    graph_t(const int size = 0) : _size(size), _adj(size) { reset(); }
    graph_t(const std::vector<T>& labels) : _size(labels.size()), _label(labels), _adj(_size) { reset(); }

    int add_node_label(T label) {
        assert(_index.count(label) == 0);
        const int ret = _size++;
        _index[label] = ret;
        _label.push_back(std::move(label));
        _adj.push_back({});
        return ret;
    }

    void detach_node_label(const T& node) {
        assert(_index.count(node));
        detach_node(_index[node]);
    }

    void detach_node(const int node) {
        assert(0 <= node && node < _size);
        for (const auto& e : _adj[node]) {
            auto& list = _adj[e[0]];
            const auto ne = std::remove_if(list.begin(), list.end(), [node](const auto& e){ return e[0] == node; });
            list.erase(ne, list.end());
        }

        _adj[node].clear();
    }

    void add_edge_label(const T& from, const T& to, const int cost = 0) {
        assert(_index.count(from) && _index.count(to));
        add_edge(_index[from], _index[to], cost);
    }

    void add_edge(const int from, const int to, const int cost = 0) {
        assert(0 <= from && from < _size && 0 <= to && to < _size);
        _adj[from].push_back({to, cost});
        _adj[to].push_back({from, cost});
    }

    void modify_edge_label(const T& from, const T& to, const int cost) {
        assert(_index.count(from) && _index.count(to));
        modify_edge(_index[from], _index[to], cost);
    }

    void modify_edge(const int from, const int to, const int cost) {
        assert(0 <= from < _size && 0 <= to < _size);
        auto& fn = _adj[from];
        const auto it = std::find_if(fn.begin(), fn.end(), [to](const auto& e){ return e[0] == to; });
        assert(it != fn.end());
        (*it)[1] = cost;
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

    void clear() {
        _size = 0;
        _adj.clear();
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

    bool found_label(const T& label) {
        assert(_index.count(label));
        return found(_index[label]);
    }

    bool found(const int node) const {
        assert(0 <= node && node < _size);
        return _pred[node] != node;
    }

    bool has_edge_label(const T& from, const T& to) const {
        assert(_index.count(from) && _index.count(to));
        return has_edge(_index[from], _index[to]);
    }

    bool has_edge(const int from, const int to) const {
        const auto& c = _adj[from];
        return std::find_if(c.begin(), c.end(), [to](const auto& e){ return e[0] == to; }) != c.end();
    }

    bool has_negative_cycle() const { return _has_neg_cycle; }

    void bfs_label(const T& from, const T& to) { // no edge cost, no requirements
        assert(_index.count(from) && _index.count(to));
        bfs(_index[from], _index[to]);
    }

    void bfs(const int source = 0, const int target = -1) {
        std::queue<int> qq;
        qq.push(source);
        _dist[source] = 0;
        _pred[source] = -1;
        _seen[source] = true;
        while (!qq.empty()) {
            const int node = qq.front(); qq.pop();
            if (node == target)
                return;
            for (const auto& e : _adj[node]) {
                const auto next = e[0];
                if (!_seen[next]) {
                    _seen[next] = true;
                    _pred[next] = node;
                    _dist[next] = _dist[node] + 1;
                    qq.push(next);
                }
            }
        }
    }

    bool check_maze() { // requires all 0 or 1 costs
        for (const auto& n : _adj)
            if (!std::all_of(n.begin(), n.end(), [](const edge_t& e){ return e[1] == 0 || e[1] == 1; }))
                return false;
        return true;
    }

    void maze_label(const T& from, const T& to) {
        assert(_index.count(from) && _index.count(to));
        maze(_index[from], _index[to]);
    }

    void maze(const int source = 0, const int target = -1) {
        std::deque<int> dq;
        dq.push_back(source);
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

    void dijkstra_label(const T& from, const T& to) {
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

    bool have_common_subpath(const int source, int i, int j) const {
        while (~_pred[i] && _pred[i] != source)
            i = _pred[i];
        while (~_pred[j] && _pred[j] != source)
            j = _pred[j];
        return ~_pred[i] && ~_pred[j] && i == j;
    }

    // first node on path from source to target, maybe target or -1
    int first_path_node(const int source, int target) const {
        while (~_pred[target] && _pred[target] != source)
            target = _pred[target];
        return target;
    }

    void bellman_ford_label(const T& from) {
        assert(_index.count(from));
        bellman_ford(_index[from]);
    }

    void bellman_ford(const int source = 0) { // negative cost accepted
        _dist[source] = 0;
        _pred[source] = -1;
        for (int z = _size; z; --z) { // one additional round
            _has_neg_cycle = false;
            for (int node = 0; node < _size; ++node)
                for (const auto& e : _adj[node]) {
                    const int next = e[0];
                    const int cand = _dist[node] + e[1];
                    if (cand < _dist[next]) {
                        _dist[next] = cand;
                        _pred[next] = node;
                        _has_neg_cycle = true;
                    }
                }
        }
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

        for (int k = 0, kr = 0; k < _size; ++k, kr += _size)
            for (int i = 0, ir = 0; i < _size; ++i, ir += _size)
                for (int j = 0, jr = 0; j < _size; ++j, jr += _size)
                    fw[ir + j] = std::min(fw[ir + j], fw[ir + k] + fw[kr + j]);

        return fw;
    }

    auto floyd_warshall(const bool check_for_neg_cycles = false) { // compute all distances
        if (check_for_neg_cycles)
            bellman_ford();
        if (_has_neg_cycle)
            return std::vector(0, std::vector<int>(0));

        std::vector<std::vector<int>> dist(_size, std::vector<int>(_size, INF));
        for (int i = 0; i < _size; ++i) {
            dist[i][i] = 0;
            for (const auto& e : _adj[i])
                dist[i][e[0]] = e[1];
        }

        for (int k = 0; k < _size; ++k)
            for (int i = 0; i < _size; ++i)
                for (int j = 0; j < _size; ++j)
                    dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);

        return dist;
    }

    int connected_components() {
        for (int source = 0; source < _size; ++source)
            if (!_seen[source]) {
                std::vector<int> ss;
                ss.push_back(source);
                _seen[source] = true;
                _cc[source] = _ccsz;
                while (!ss.empty()) {
                    const int node = ss.back(); ss.pop_back();
                    for (const auto& e : _adj[node]) {
                        const auto next = e[0];
                        if (!_seen[next]) {
                            _seen[next] = true;
                            _cc[next] = _ccsz;
                            ss.push_back(next);
                        }
                    }
                }
                ++_ccsz;
            }

        return _ccsz;
    }

    bool check_euler(const int source = 0) { // all edges exists <=> e[1] > 0
        int odd_nodes{0};
        for (const auto& n : _adj) {
            if (!std::all_of(n.begin(), n.end(), [](const edge_t& e){ return e[1] > 0; }))
                return false;
            if (n.size() & 1)
                ++odd_nodes;
        }

        return odd_nodes == 0 || (odd_nodes == 2 && (_adj[source].size() & 1));
    }

    auto euler_path_label(const T& from) {
        assert(_index.count(from));
        return euler_path(_index[from]);
    }

    auto euler_path(const int source = 0) {
        std::vector<T> path;
        std::vector<int> stack(1, source);
        while (!stack.empty()) {
            const int node = stack.back();
            auto& neibors = _adj[node];
            const auto next = std::find_if(neibors.begin(), neibors.end(), [](const auto& e){ return e[1] != 0; });
            if (next == neibors.end()) {
                stack.pop_back();
                path.push_back(_label[node]);
            } else {
                (*next)[1] = 0;
                for (auto& be : _adj[(*next)[0]]) // see: \usaco\chapter3\fence.cpp
                    if (be[0] == node) {
                        be[1] = 0;
                        break;
                    }
                stack.push_back((*next)[0]);
            }
        }

        return path;
    }

    auto find_bridges() {
        std::vector<std::pair<T, T>> ret;
        for (int n = 0; n < _size; ++n)
            if (!_seen[n])
                bridge_dfs(n, -1, ret);

        return ret;
    }

    static constexpr const int INF = (1 << 30) - 1;

  private:
    int _size, _ccsz, _time;
    bool _has_neg_cycle;
    std::vector<T> _label;
    std::vector<bool> _seen;
    std::vector<int> _cc, _low; // concomp index, low table for find_bridges
    std::unique_ptr<int[]> _fw_uptr;
    std::vector<int> _dist, _pred, _order;
    std::map<T, int> _index; // unordered will not work with unhashable T
    std::vector<std::vector<edge_t>> _adj;

    void bridge_dfs(const int n, const int p, std::vector<std::pair<T, T>>& ret) { // node, parent
        _seen[n] = true;
        _order[n] = _low[n] = _time++;
        for (const auto& [to, cost] : _adj[n]) {
            if (to == p)
                continue;
            if (_seen[to])
                _low[n] = std::min(_low[n], _order[to]);
            else {
                bridge_dfs(to, n, ret);
                _low[n] = std::min(_low[n], _low[to]);
                if (_order[n] < _low[to])
                    ret.emplace_back(_label[n], _label[to]);
            }
        }
    }
};

graph_t<char> g;
graph_t<int> ig;

void init_maze()
{
    g.clear();
    for (const char c : "ABCD")
        if (c) g.add_node_label(c);

    g.add_edge_label('A', 'B', 1);
    g.add_edge_label('B', 'C', 0);
    g.add_edge_label('C', 'D', 0);
    g.add_edge_label('B', 'D', 1);
    g.reset();
}

void init_dijkstra()
{
    g.clear();
    for (const char c : "ABCDEFGHIJKLM")
        if (c) g.add_node_label(c);

    g.add_edge_label('A', 'B', 3);
    g.add_edge_label('A', 'C', 1);
    g.add_edge_label('B', 'E', 2);
    g.add_edge_label('B', 'F', 1);
    g.add_edge_label('C', 'F', 2);
    g.add_edge_label('C', 'G', 1);
    g.add_edge_label('D', 'H', 1);
    g.add_edge_label('C', 'D', 2);
    g.add_edge_label('D', 'I', 2);
    g.add_edge_label('G', 'H', 3);
    g.add_edge_label('H', 'I', 1);
    g.add_edge_label('E', 'J', 2);
    g.add_edge_label('F', 'J', 3);
    g.add_edge_label('G', 'K', 1);
    g.add_edge_label('H', 'L', 1);
    g.add_edge_label('K', 'L', 4);
    g.add_edge_label('J', 'M', 1);
    g.add_edge_label('K', 'M', 2);
    g.reset();
}

void init_euler()
{
    g.clear();
    for (const char c : "ABCDEFG")
        if (c) g.add_node_label(c);

    g.add_edge_label('A', 'D', 1);
    g.add_edge_label('A', 'E', 1);
    g.add_edge_label('B', 'D', 1);
    g.add_edge_label('B', 'E', 1);
    g.add_edge_label('B', 'F', 1);
    g.add_edge_label('B', 'G', 1);
    g.add_edge_label('C', 'D', 1);
    g.add_edge_label('C', 'G', 1);
    g.add_edge_label('D', 'F', 1);
    g.add_edge_label('E', 'F', 1);
    g.add_edge_label('E', 'G', 1);
    g.add_edge_label('F', 'G', 1);
    g.reset();
}

void init_bridges() {
    ig.clear();
    for (int i = 10, z = 3; z; --z, i *= 10) 
        for (int y = 3; y; --y)
            ig.add_node_label(i + y - 1);

    ig.add_edge_label(10, 11); ig.add_edge_label(12, 11); ig.add_edge_label(10, 12); 
    ig.add_edge_label(100, 101); ig.add_edge_label(102, 101); ig.add_edge_label(100, 102); 
    ig.add_edge_label(1000, 1001); ig.add_edge_label(1002, 1001); ig.add_edge_label(1000, 1002); 
    ig.add_edge_label(12, 102); ig.add_edge_label(11, 1001); // bridges
    ig.reset();
}

int main(int argc, char* argv[])
{
    init_maze();
    g.bfs_label('A', 'D');
    assert((g.get_path_to_label('D') == std::vector{'A', 'B', 'D'}) && g.get_cost_to_label('D') == 2);

    g.reset();
    if (g.check_maze())
        g.maze_label('A', 'D');
    assert((g.get_path_to_label('D') == std::vector{'A', 'B', 'C', 'D'}) && g.get_cost_to_label('D') == 1);

    g.reset();
    g.bellman_ford_label('A');
    assert(!g.has_negative_cycle() && (g.get_path_to_label('D') == std::vector{'A', 'B', 'C', 'D'}) && g.get_cost_to_label('D') == 1);
    g.reset();
    g.modify_edge_label('B', 'D', -1);
    g.bellman_ford_label('A');
    assert(g.has_negative_cycle());

    init_dijkstra();
    if (g.check_dijkstra())
        g.dijkstra_label('A', 'M');

    if (!g.found_label('M'))
        std::cout << "\nPath not found\n";
    else {
        const auto path = g.get_path_to_label('M');
        const auto length = int(path.size());
        std::cout << "\nFound shortest path from A to M with the length of " << length << " and cost " << g.get_cost_to_label('M') << '\n';
        std::cout << path.front();
        for (int i = 1; i < length; ++i)
            std::cout << "->" << path[i];
        std::cout << '\n';
    }

    const auto dist = g.floyd_warshall();
    assert(!dist.empty());
    assert(dist['A' - 'A']['J' - 'A'] == 6);
    assert(dist['I' - 'A']['E' - 'A'] == 9);

    g.add_node_label('N');
    g.add_node_label('O');
    g.add_edge_label('N', 'O', 42);
    g.reset();
    assert(g.connected_components() == 2);

    init_euler();
    if (g.check_euler()) {
        const auto path = g.euler_path_label('A');
        for (const auto l : path)
            std::cout << l << ' ';
        std::cout << '\n';
    }

    init_bridges();
    const auto bridges = ig.find_bridges();
    for (const auto& b : bridges)
        std::cout << b.first << '-' << b.second << ' ';
    std::cout << "\nPASSED\n";
}

/*    clang++.exe -Wall -g -std=c++17 graph.cpp -o graph.exe

Output:

Found shortest path from A to M with the length of 5 and cost 5
    A->C->G->K->M
    A E G F D C G B F E B D A
    11-1001 12-102

*/

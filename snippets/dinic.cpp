/**
 *    author:  tourist
 *    created: 05.06.2021 17:25:36
 *    usage sample: /codejam/2021/square_free.cpp
**/

#include <cassert>
#include <limits>
#include <vector>

template <typename T>
class flow_graph_t
{
  public:
    static constexpr T eps = (T)1e-9;

    struct edge_t {
        int from;
        int to;
        T capacity;
        T flow;
    };

    std::vector<std::vector<int>> g;
    std::vector<edge_t> edges;
    int n;
    int st;
    int fin;
    T flow;

    flow_graph_t(int _n, int _st, int _fin) : n(_n), st(_st), fin(_fin) {
        assert(0 <= st && st < n && 0 <= fin && fin < n && st != fin);
        g.resize(n);
        flow = 0;
    }

    void clear_flow() {
        flow = 0;
        for (const auto& e : edges)
            e.flow = 0;
    }

    int add(int from, int to, T forward_cap, T backward_cap = T{}) {
        assert(0 <= from && from < n && 0 <= to && to < n);
        int id = (int)edges.size();
        g[from].push_back(id);
        edges.push_back({from, to, forward_cap, 0});
        g[to].push_back(id + 1);
        edges.push_back({to, from, backward_cap, 0});
        return id;
    }
};

/**
 * @brief Works for one unit capacity flow graphs
 */
template <typename T>
class dinic_t
{
  public:
    flow_graph_t<T>& _fg;

    std::vector<int> ptr;
    std::vector<int> d;
    std::vector<int> q;

    dinic_t(flow_graph_t<T>& fg) : _fg(fg) {
        ptr.resize(_fg.n);
        d.resize(_fg.n);
        q.resize(_fg.n);
    }

    bool expath() {
        fill(d.begin(), d.end(), -1);
        q[0] = _fg.fin;
        d[_fg.fin] = 0;
        int beg = 0, end = 1;
        while (beg < end) {
            const int i = q[beg++];
            for (int id : _fg.g[i]) {
                const auto& e = _fg.edges[id];
                const auto& back = _fg.edges[id ^ 1];
                if (back.capacity - back.flow > _fg.eps && d[e.to] == -1) {
                    d[e.to] = d[i] + 1;
                    if (e.to == _fg.st)
                        return true;

                    q[end++] = e.to;
                }
            }
        }

        return false;
    }

    T dfs(int v, T w) {
        if (v == _fg.fin)
            return w;

        int& j = ptr[v];
        while (j >= 0) {
            const int id = _fg.g[v][j];
            const auto& e = _fg.edges[id];
            if (e.capacity - e.flow > _fg.eps && d[e.to] == d[v] - 1) {
                T t = dfs(e.to, std::min(e.capacity - e.flow, w));
                if (t > _fg.eps) {
                    _fg.edges[id].flow += t;
                    _fg.edges[id ^ 1].flow -= t;
                    return t;
                }
            }
            --j;
        }

        return 0;
    }

    T max_flow() {
        while (expath()) {
            for (int i = 0; i < _fg.n; ++i)
                ptr[i] = (int)_fg.g[i].size() - 1;

            T big_add{0};
            while (true) {
                T add = dfs(_fg.st, std::numeric_limits<T>::max());
                if (add <= _fg.eps)
                    break;
                big_add += add;
            }
            if (big_add <= _fg.eps)
                break;
            _fg.flow += big_add;
        }

        return _fg.flow;
    }

    std::vector<bool> min_cut() {
        max_flow();
        std::vector<bool> ret(_fg.n);
        for (int i = 0; i < _fg.n; ++i)
            ret[i] = (d[i] != -1);

        return ret;
    }
};

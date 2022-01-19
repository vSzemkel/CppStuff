#include <queue>
#include <utility>
#include <vector>

std::vector<std::vector<std::pair<int, int>>> graph; // {to, cost}

static void shortest_paths(int source, std::vector<int>& distances)
{
    std::vector<bool> in_queue(V);
    std::queue<int> q;
    q.push(source);
    distances.assign(V, INF);
    distances[source] = 0;

    while (!q.empty()) {
        const int cur = q.front(); q.pop();
        in_queue[cur] = false;
        for (const auto& [next, cost] : graph[cur])
            if (distances[cur] + cost < distances[next]) {
                distances[next] = distances[cur] + cost;
                if (!in_queue[next]) {
                    q.push(next);
                    in_queue[next] = true;
                }
            }
    }
}

**********************************************************

std::vector<std::vector<int>> g_adj, g_cost, g_capacity; // [src][dst] == attribute

static void shortest_paths(int source, std::vector<int>& distances, std::vector<int>& predecors)
{
    assert(0 <= source && source < (int)g_cost.size());
    std::vector<bool> in_queue(g_V, false);
    std::queue<int> q;
    q.push(source);
    predecors.assign(g_V, -1);
    distances.assign(g_V, INF);
    distances[source] = 0;

    while (!q.empty()) {
        const int cur = q.front(); q.pop();
        in_queue[cur] = false;
        for (const auto next : g_adj[cur])
            if (distances[cur] + g_cost[cur][next] < distances[next] && g_capacity[cur][next] > 0) {
                distances[next] = distances[cur] + g_cost[cur][next];
                predecors[next] = cur;
                if (!in_queue[next]) {
                    q.push(next);
                    in_queue[next] = true;
                }
            }
    }
}

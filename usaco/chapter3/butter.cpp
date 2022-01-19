/*
ID: marcin.8
LANG: C++
TASK: butter
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=rrU580NOXFC&S=butter
*/

#include <fstream>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

std::ifstream task_in("butter.in");
std::ofstream task_out("butter.out");

int N, V, E;
const int INF = 1e09;
std::unordered_map<int, int> counts; // counts[P] == N; N cows on pasture P
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

/**
 * @brief Any pasture potentially can be optimal
 * This could also be done with floyd_warshall
 */
int main(int, char**)
{
    task_in >> N >> V >> E;
    for (int i = 0; i < N; ++i) {
        int p; task_in >> p; --p;
        ++counts[p];
    }
    graph.resize(V);
    for (int i = 0; i < E; ++i) {
        int f, t, c; task_in >> f >> t >> c; --f; --t;
        graph[f].emplace_back(t, c);
        graph[t].emplace_back(f, c);
    }

    int min{INF};
    for (int p = 0; p < V; ++p) {
        std::vector<int> distances;
        shortest_paths(p, distances);
        int can{0};
        for (const auto& [t, c] : counts)
            can += distances[t] * c;
        if (can < min)
            min = can;
    }

    task_out << min << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 butter.cpp -o butter.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address butter.cpp -o butter

Input:

3 4 5
2
3
4
1 2 1
1 3 5
2 3 7
2 4 3
3 4 5

Output:

8

*/

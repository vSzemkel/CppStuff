/*
ID: marcin.8
LANG: C++
TASK: comehome
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=A0kIGYpcC8f&S=comehome
*/

#include <algorithm>
#include <array>
#include <assert.h>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>

const int INF = 1e09;

std::ifstream task_in("comehome.in");
std::ofstream task_out("comehome.out");

std::vector<std::vector<std::array<int, 2>>> graph; // node -> {neigbor, dist}

static void shortest_paths(const int source, std::vector<int>& distances)
{
    const int V = int(graph.size());
    assert(0 <= source && source < V);
    std::vector<bool> in_queue(V, false);
    std::queue<int> q;
    q.push(source);
    distances.assign(V, INF);
    distances[source] = 0;

    while (!q.empty()) {
        const int cur = q.front(); q.pop();
        in_queue[cur] = false;
        for (const auto [next, dist] : graph[cur])
            if (distances[cur] + dist < distances[next]) {
                distances[next] = distances[cur] + dist;
                if (!in_queue[next]) {
                    q.push(next);
                    in_queue[next] = true;
                }
            }
    }
}

int main(int, char**)
{
    int N; task_in >> N;
    std::vector<bool> cows; // node with cow
    std::vector<char> rev_map; // node -> label
    std::unordered_map<char, int> map; // label -> node
    for (int i = 0; i < N; ++i) {
        char from, to; int dist;
        task_in >> from >> to >> dist;
        bool is_upper = from < 'a';
        auto found = map.find(from);
        if (found == map.end()) {
            map[from] = cows.size();
            rev_map.push_back(from);
            cows.push_back(is_upper && from != 'Z');
        }
        is_upper = to < 'a';
        found = map.find(to);
        if (found == map.end()) {
            map[to] = cows.size();
            rev_map.push_back(to);
            cows.push_back(is_upper && to != 'Z');
        }
        graph.resize(rev_map.size());
        graph[map[from]].push_back({map[to], dist});
        graph[map[to]].push_back({map[from], dist});
    }

    std::vector<int> dist;
    shortest_paths(map['Z'], dist);
    for (int i = 0; i < int(cows.size()); ++i)
        if (!cows[i]) dist[i] = INF;
    const auto max = std::min_element(dist.begin(), dist.end()) - dist.begin();
    task_out << rev_map[max] << ' ' << dist[max] << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 comehome.cpp -o comehome.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address comehome.cpp -o comehome

Input:

5
A d 6
B d 3
C e 9
d Z 8
e Z 3

Output:

B 11

*/

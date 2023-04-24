
#include <iostream>
#include <queue>
#include <unordered_set>
#include <utility>
#include <vector>

// Monsters
// https://codeforces.com/contest/1810/problem/E

static void solve_slow() {
    int V, E;
    std::cin >> V >> E;
    std::queue<int> q;
    std::vector<int> dangers(V);
    for (auto& d : dangers)
        std::cin >> d;
    std::vector<std::vector<int>> graph(V);
    for (int z = E; z; --z) {
        int f, t;
        std::cin >> f >> t;
        --f, --t;
        graph[f].push_back(t);
        graph[t].push_back(f);
    }

    std::unordered_set<int> checked_starts;
    for (int start = 0; start < V; ++start)
        if (dangers[start] == 0 && !checked_starts.contains(start)) {
            std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> reachable_alive; // {denger, index}
            std::vector<bool> was_enqued(V);
            int dead{0};
            q.push(start);
            was_enqued[start] = true;
            while (!q.empty()) {
                const int cur = q.front();
                q.pop();
                ++dead;
                if (dangers[cur] == 0)
                    checked_starts.insert(cur);
                for (const int next : graph[cur])
                    if (!was_enqued[next]) {
                        if (dangers[next] <= dead) {
                            q.push(next);
                            was_enqued[next] = true;
                        } else
                            reachable_alive.emplace(dangers[next], next);
                    }

                while (!reachable_alive.empty() && reachable_alive.top().first <= dead) {
                    const int can = reachable_alive.top().second;
                    if (!was_enqued[can]) {
                        q.push(can);
                        was_enqued[can] = true;
                    }
                    reachable_alive.pop();
                }
            }

            if (dead == V) {
                std::cout << "YES";
                return;
            }
        }

    std::cout << "NO";
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        solve_slow(); std::cout << '\n';
    }
}

/*

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 monsters.cpp -o monsters.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address monsters.cpp -o monsters

Run:
py.exe interactive_runner.py py.exe monsters_testing_tool.py 1 -- monsters.exe
monsters.exe < monsters.in

Input:

5
4 3
2 1 0 3
1 2
2 3
3 4
6 6
0 1 2 3 0 1
1 2
2 3
3 4
4 5
4 6
5 6
4 3
0 1 2 0
1 2
2 3
1 3
4 6
1 1 1 0
1 2
3 2
4 3
2 4
4 1
1 3
5 5
0 1 3 2 0
1 2
2 3
3 4
4 5
3 5

Output:


*/

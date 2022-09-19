
#include <iostream>
#include <tuple>
#include <vector>

// Water Container System
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb409/0000000000bef79e

static void solve() {
    int N, Q;
    std::cin >> N >> Q;
    std::vector<std::vector<int>> graph(N);
    for (int z = N - 1; z; --z) {
        int a, b; std::cin >> a >> b;
        --a; --b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    std::vector<bool> seen(N);
    std::vector<int> level_size(N, 0);
    std::vector<std::pair<int, int>> stack; // {node, level}
    stack.emplace_back(0, 0);
    seen[0] = true;
    while (!stack.empty()) {
        const auto [node, level] = stack.back();
        stack.pop_back();
        ++level_size[level];
        for (const auto next : graph[node]) {
            if (!seen[next]) {
                stack.emplace_back(next, level + 1);
                seen[next] = true;
            }
        }
    }

    for (int _, z = Q; z; --z)
        std::cin >> _; // ignore

    int ret{0}, level{0};
    while (ret < Q) {
        if (Q - ret < level_size[level])
            break;
        ret += level_size[level];
        ++level;
    }

    std::cout << ret;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 water_container_system.cpp -o water_container_system.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address water_container_system.cpp -o water_container_system

Run:
water_container_system.exe < water_container_system.in

Input:

4
1 1
1
3 2
1 2
1 3
1
2
4 4
1 2
1 3
2 4
3
3
3
3
5 2
1 2
5 3
3 1
2 4
4
5

Output:

Case #1: 1
Case #2: 1
Case #3: 4
Case #4: 1

*/

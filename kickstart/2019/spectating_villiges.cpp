
#include <algorithm>
#include <iostream>
#include <functional>
#include <vector>

// Spectating Villiges
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050edc/000000000018666b

constexpr const int64_t INF = 1e18;

static void solve() {
    int N;
    std::cin >> N;
    std::vector<int64_t> beauty(N);
    for (auto& b :beauty)
        std::cin >> b;
    std::vector<std::vector<int>> tree(N);
    for (int z = N - 1; z; --z) {
        int v0, v1;
        std::cin >> v0 >> v1;
        --v0, --v1;
        tree[v0].push_back(v1);
        tree[v1].push_back(v0);
    }

    std::vector<int64_t> has_light = beauty;
    std::vector<int64_t> was_lightened(N, -INF);
    std::vector<int64_t> node_and_child_black(N);
    std::function<void(int, int)> dfs = [&](int node, int parent) {
        for (const int child : tree[node])
            if (child != parent) { // by Radewoosh
                dfs(child, node);

                const auto hl = has_light[node];
                has_light[node] = -INF;
                const auto wl = was_lightened[node];
                was_lightened[node] = -INF;
                const auto nacb = node_and_child_black[node];
                node_and_child_black[node] = -INF;

                has_light[node] = std::max(has_light[node], hl + has_light[child]);
                has_light[node] = std::max(has_light[node], hl + was_lightened[child]);
                has_light[node] = std::max(has_light[node], hl + beauty[child] + node_and_child_black[child]);

                was_lightened[node] = std::max(was_lightened[node], wl + has_light[child]);
                was_lightened[node] = std::max(was_lightened[node], wl + was_lightened[child]);
                was_lightened[node] = std::max(was_lightened[node], wl + node_and_child_black[child]);

                was_lightened[node] = std::max(was_lightened[node], nacb + beauty[node] + has_light[child]);
                node_and_child_black[node] = std::max(node_and_child_black[node], nacb + was_lightened[child]);
                node_and_child_black[node] = std::max(node_and_child_black[node], nacb + node_and_child_black[child]);
            }
    };

    dfs(0, -1);

    std::cout << std::max({has_light[0], was_lightened[0], node_and_child_black[0]});
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
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++17 spectating_villiges.cpp -o spectating_villiges.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address spectating_villiges.cpp -o spectating_villiges

Run:
spectating_villiges.exe < spectating_villiges.in

Input:

3
9
-10 4 -10 8 20 30 -2 -3 7
1 4
2 4
4 3
9 4
9 8
7 5
6 7
7 9
4
-2 20 20 20
1 2
1 3
1 4
5
-5 -10 8 -7 -2
5 4
4 3
3 2
2 1

Output:

Case #1: 67
Case #2: 58
Case #3: 0

*/

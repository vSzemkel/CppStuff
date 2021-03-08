
#include <iostream>
#include <string>
#include <utility>
#include <vector>

// You can go your own way
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000051705/00000000000881da

int64_t g_N;

static void solve() {
    std::cin >> g_N;
    std::string lydia_path; std::cin >> lydia_path;
    for (char& c : lydia_path)
        c = (c =='S') ? 'E' : 'S';
    std::cout << lydia_path << '\n';
}

static void brutforce_dfs() {
    std::cin >> g_N;
    std::string lydia_path; std::cin >> lydia_path;

    const auto path_index = [](const int64_t position) noexcept {
        return (position / g_N) + (position % g_N) - 1;
    };

    // mark Lydia's path
    int row{0}, col{0};
    std::vector<bool> lydia(g_N * g_N);
    lydia[0] = true;
    for (const char c : lydia_path) {
        if (c == 'S') ++row;
        else ++col;
        lydia[row * g_N + col] = true;
    }

    std::string path{lydia_path};
    std::vector<bool> visited(g_N * g_N);
    std::vector<std::pair<int64_t, int64_t>> dfsq;
    dfsq.emplace_back(0, -1);
    while (!dfsq.empty()) {
        const auto pos = dfsq.back(); dfsq.pop_back();
        const int64_t cur = pos.first;
        if (cur > 0)
            path[path_index(cur)] = (pos.first - pos.second > 1) ? 'S' : 'E';
        if (cur == g_N * g_N - 1)
            break;
        // try E
        const int64_t tryE = cur + 1;
        if (tryE % g_N != 0 && !visited[tryE])
            if (!lydia[tryE] || lydia_path[path_index(tryE)] != 'E') {
                visited[tryE] = true;
                dfsq.emplace_back(tryE, cur);
            }
        // try S
        const int64_t tryS = cur + g_N;
        if (tryS / g_N < g_N && !visited[tryS])
            if (!lydia[tryS] || lydia_path[path_index(tryS)] != 'S') {
                visited[tryS] = true;
                dfsq.emplace_back(tryS, cur);
            }
    }

    std::cout << path << '\n';
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve();
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 you_can_go_own_way.cpp -o you_can_go_own_way.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 you_can_go_own_way.cpp -o you_can_go_own_way.o

Run:
you_can_go_own_way.exe < you_can_go_own_way.in

Input:

2
2
SE
5
EESSSESE

Output:


*/

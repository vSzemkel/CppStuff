/*
ID: marcin.8
LANG: C++
TASK: cowtour
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=HMDfB318J5w&S=cowtour
*/

#include <algorithm>
#include <array>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <vector>

std::ifstream task_in("cowtour.in");
std::ofstream task_out("cowtour.out");

constexpr const double INF = 1e09;

int main(int, char**)
{
    int N; task_in >> N;
    std::vector<std::array<int, 2>> coord(N);
    for (auto& c : coord)
        task_in >> c[0] >> c[1];
    std::vector<std::vector<double>> dist(N, std::vector<double>(N));
    std::vector<std::vector<double>> pathdist(N, std::vector<double>(N, INF));
    std::vector<std::vector<bool>> graph(N, std::vector<bool>(N));
    for (int i = 0; i < N; ++i) {
        pathdist[i][i] = 0;
        for (int j = 0; j < N; ++j) {
            char c; task_in >> c;
            const bool connected = c == '1';
            graph[i][j] = connected;
            const int dx = coord[i][0] - coord[j][0];
            const int dy = coord[i][1] - coord[j][1];
            const double len = std::sqrt((dx * dx) + (dy * dy));
            dist[i][j] = len;
            if (connected)
                pathdist[i][j] = len;
        }
    }

    // floyd-warshall
    for (int k = 0; k < N; ++k)
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                pathdist[i][j] = std::min(pathdist[i][j], pathdist[i][k] + pathdist[k][j]);

    // connected components
    int next_cmp{0};
    std::vector<int> comp(N);
    for (int i = 0; i < N; ++i)
        if (comp[i] == 0) {
            ++next_cmp;
            std::vector<int> stack(1, i);
            while (!stack.empty()) {
                const int cur = stack.back();
                stack.pop_back();
                comp[cur] = next_cmp;
                for (int next = 0; next < N; ++next)
                    if (graph[cur][next] && comp[next] == 0)
                        stack.push_back(next);
            }
        }

    // diameters
    std::vector<double> diam(N); // longest path in cc from [i]
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (comp[i] == comp[j])
                diam[i] = std::max(diam[i], pathdist[i][j]);
    std::vector<double> ccdiam(next_cmp + 1); // longest path in cc
    for (int c = 1; c <= next_cmp; ++c)
        for (int i = 0; i < N; ++i)
            if (comp[i] == c)
                ccdiam[c] = std::max(ccdiam[c], diam[i]);

    double ret{INF};
    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j)
            if (comp[i] != comp[j])
                ret = std::min(ret, diam[i] + dist[i][j] + diam[j]);
    for (const auto inner : ccdiam) // maybe longest path is in single cc
        ret = std::max(ret, inner);

    task_out << std::fixed << std::setprecision(6) << ret << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 cowtour.cpp -o cowtour.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address cowtour.cpp -o cowtour

Input:

8
10 10
15 10
20 10
15 15
20 15
30 15
25 10
30 10
01000000
10111000
01001000
01001000
01110000
00000010
00000101
00000010

Output:

22.071068

*/

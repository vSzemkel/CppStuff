/*
ID: marcin.8
LANG: C++
TASK: wormhole
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=Re1euIbhzhZ&S=wormhole
*/

#include <algorithm>
#include <assert.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

std::ifstream task_in("wormhole.in");
std::ofstream task_out("wormhole.out");

std::vector<bool> visited;
std::vector<int> reachable, wormable;

static double factorial(const int n) {
    return std::tgamma(n + 1);
}

static bool has_cycle(const int start, const int target, const bool use_wormhole) {
    const int dest = use_wormhole ? wormable[start] : reachable[start];
    if (dest >= 0) {
        if (use_wormhole && dest == target)
            return true;
        if (!visited[dest]) {
            visited[dest] = true;
            if (has_cycle(dest, target, !use_wormhole))
                return true;
        }
    }

    return false;
}

int main(int, char**)
{
    int N; task_in >> N;
    assert((N & 1) == 0);
    const int P = N / 2;
    // const int MAX = factorial(N) / (1 << P);
    std::vector<std::pair<int, int>> points(N), pair_indexes;
    wormable.resize(N);
    for (auto& p : points)
        task_in >> p.first >> p.second;
    std::vector<int> pairing(N);
    for (int i = 0; i < N; i += 2)
        pairing[i] = pairing[i + 1] = i / 2;
    std::sort(points.begin(), points.end(), [](const auto& p1, const auto& p2){
        return p1.second < p2.second || (p1.second == p2.second && p1.first < p2.first);
    });

    reachable.assign(N, -1);
    for (int i = 1; i < N; ++i)
        if (points[i - 1].second == points[i].second)
            reachable[i - 1] = i;

    int ret{0};
    do {
        pair_indexes.assign(P, {-1, -1});
        for (int i = 0; i < N; ++i) {
            auto& pair = pair_indexes[pairing[i]];
            if (pair.first == -1) {
                pair.first = i;
            } else {
                pair.second = i;
                wormable[pair.first] = i;
                wormable[i] = pair.first;
            }
        }

        if (std::is_sorted(pair_indexes.begin(), pair_indexes.end()))
            for (const auto& w : pair_indexes) {
                visited.assign(N, false);
                if (has_cycle(w.first, w.first, false)) {
                    ++ret;
                    break;
                }
                visited.assign(N, false);
                if (has_cycle(w.second, w.second, false)) {
                    ++ret;
                    break;
                }
            }
    } while (std::next_permutation(pairing.begin(), pairing.end()));

    task_out << ret << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 wormhole.cpp -o wormhole.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address wormhole.cpp -o wormhole

Input:


Output:


*/

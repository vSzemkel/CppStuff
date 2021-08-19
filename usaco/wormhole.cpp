/*
ID: marcin.8
LANG: C++
TASK: wormhole
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=Re1euIbhzhZ&S=wormhole
*/

#include <algorithm>
#include <array>
#include <assert.h>
#include <bitset>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <filesystem>
#include <fstream>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <stdlib.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <vector>

std::ifstream task_in("wormhole.in");
std::ofstream task_out("wormhole.out");

std::vector<bool> visited;
std::vector<std::pair<int, int>> points;
std::vector<int> pairing, reachable, wormable;

static double factorial(const int n) {
    return std::tgamma(n + 1);
}

static bool has_cycle(const int start, const int target, const bool use_wormhole) {
    const int dest = use_wormhole ? wormable[start] : reachable[start];
    if (dest >= 0) {
        if (dest == target)
            return true;
        if (!visited[dest]) {
            visited[dest] = true;
            if (has_cycle(dest, target, !use_wormhole))
                return true;
        }
    }

    return false;
}

static std::pair<int, int> get_pair(const int p) { // returns indexes of points in pair p
    const auto first = std::find(pairing.begin(), pairing.end(), p) - pairing.begin();
    const auto second = std::find(pairing.begin() + first + 1, pairing.end(), p) - pairing.begin();
    return {first, second};
}

int main(int, char**)
{
    int N; task_in >> N;
    assert((N & 1) == 0);
    const int P = N / 2;
    points.resize(N);
    pairing.resize(N);
    wormable.resize(N);
    for (auto& p : points)
        task_in >> p.first >> p.second;
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
        std::vector<std::pair<int, int>> pair_indexes(P);
        for (int p = 0; p < P; ++p) {
            const auto& w = pair_indexes[p] = get_pair(p);
            wormable[w.first] = w.second;
            wormable[w.second] = w.first;
        }

        for (auto p : pairing) std::cout << p << ' '; 
        std::cout << '\n';
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

    task_out << ret / int(factorial(P)) << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 wormhole.cpp -o wormhole.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address wormhole.cpp -o wormhole

Input:


Output:


*/

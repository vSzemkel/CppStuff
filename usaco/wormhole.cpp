/*
ID: marcin.8
LANG: C++
TASK: wormhole
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=Re1euIbhzhZ&S=wormhole
*/

// key observation: a segment in a cycle is a pair {holes pair, type of travel}
// maximal cycle length is <= 2 * N

#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

std::ifstream task_in("wormhole.in");
std::ofstream task_out("wormhole.out");

// alternative, recursive solution, not using std::next_permutation
// int solve(void) 
// {
//   // find first unpaired wormhole
//   int i, ret{0};
//   for (i = 0; i < N; ++i) 
//     if (wormable[i] == -1) break;
// 
//   // everyone paired?
//   if (i >= N)
//     return cycle_exists() ? 1 : 0;
// 
//   // try pairing i with all possible other wormholes j
//   for (int j = i + 1; j < N; ++j)
//     if (wormable[j] == -1) {
//       // try pairing i & j, let recursion continue to 
//       // generate the rest of the solution
//       wormable[i] = j;
//       wormable[j] = i;
//       ret += solve();
//       wormable[i] = wormable[j] = -1;
//     }
//   return ret;
// }

int main(int, char**)
{
    int N; task_in >> N;
    assert((N & 1) == 0);
    const int P = N / 2;
    std::vector<std::pair<int, int>> points(N);
    for (auto& p : points)
        task_in >> p.first >> p.second;
    std::vector<int> pairing(N), pair_indexes;
    for (int i = 0; i < N; i += 2)
        pairing[i] = pairing[i + 1] = i / 2;
    std::sort(points.begin(), points.end(), [](const auto& p1, const auto& p2){
        return p1.second < p2.second || (p1.second == p2.second && p1.first < p2.first);
    });
    // Bessie will walk from P to reachable[P], -1 if she has clear path
    std::vector<int> wormable(N), reachable(N, -1);
    for (int i = 1; i < N; ++i)
        if (points[i - 1].second == points[i].second)
            reachable[i - 1] = i;

    int ret{0};
    do {
        // For every wormhole permutation, Bessie will jump from P to wormable[P]
        pair_indexes.assign(P, -1);
        for (int i = 0; i < N; ++i) {
            auto& pair = pair_indexes[pairing[i]];
            if (pair == -1)
                pair = i;
            else {
                wormable[pair] = i;
                wormable[i] = pair;
            }
        }

        // wormhole pairs are indistinguishable, take them in any order
        if (std::is_sorted(pair_indexes.begin(), pair_indexes.end()))
            for (int i = 0; i < N; ++i) {
                int cur{i}; // check for cycle starting in cur
                for (int c = 0; c < N; ++c) {
                    cur = reachable[wormable[cur]]; // order is important
                    if (cur < 0) break;
                }
                if (cur >= 0) {
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

12 
0 0 
1 0 
2 0 
3 0 
4 0 
5 0 
6 0 
7 0 
8 0 
9 0 
10 0
10 1

Output:

8910

*/

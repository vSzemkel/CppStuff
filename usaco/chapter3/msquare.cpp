/*
ID: marcin.8
LANG: C++
TASK: msquare
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=S7oSd4llX1F&S=msquare
*/

#include <algorithm>
#include <array>
#include <assert.h>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <vector>

#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>  // Not just <immintrin.h> for compilers other than icc
#endif

std::ifstream task_in("msquare.in");
std::ofstream task_out("msquare.out");

struct game_t : std::array<char, 8> {
    game_t(const int64_t val = 0) { *(int64_t*)data() = val; }
    void operator=(const int64_t val) { *(int64_t*)data() = val; }
    operator int64_t() const { return *(int64_t*)data(); }
};

static game_t A(game_t g) {
    int& low = *(int*)g.data();
    int& hi = *(int*)(g.data() + 4);
    std::swap(low, hi);
    return g;
}

static game_t B(game_t g) {
    int& low = *(int*)g.data();
    low = _rotl(low, 8);
    int& hi = *(int*)(g.data() + 4);
    hi = _rotl(hi, 8);
    return g;
}

static game_t C(game_t g) {
    const auto stage = g[1];
    g[1] = g[5];
    g[5] = g[6];
    g[6] = g[2];
    g[2] = stage;
    return g;
}

/**
 * @brief Data collected by fuzzing, not needed in ultimate solution
 * AA ->
 * BA -> AB
 * ABA -> B
 * BAB -> ABB
 * BBA -> ABB
 * CAC -> A
 * CCA -> ACC
 * CCC -> ACA
 * BBBB ->
 * CCCC ->
 */
int main(int, char**)
{
    constexpr const char letter[] = "ABC";
    const game_t init = *(int64_t*)"12348765";

    game_t target;
    for (auto& t : target)
        task_in >> t;
    std::reverse(target.begin() + 4, target.end());

    game_t perm = *(int64_t*)"12345678";
    std::unordered_map<int64_t, std::array<game_t, 3>> graph;
    do {
        graph[perm] = {A(perm), B(perm), C(perm)};
    } while (std::next_permutation(perm.begin(), perm.end()));

    std::queue<game_t> qq;
    std::unordered_map<int64_t, std::pair<int64_t, char>> parent; // {from, edge}
    qq.push(init);
    while (!qq.empty()) {
        auto& node = qq.front();
        if (node == target)
            break;
        for (int i = 0; i < 3; ++i) {
            const auto& child = graph[node][i];
            if (parent[child].first == 0) {
                parent[child] = std::pair<int64_t, char>{node, letter[i]};
                qq.push(child);
            }
        }
        qq.pop();
    }

    std::string path;
    for (auto p = target; p != init; p = parent[p].first)
        path += parent[p].second;

    std::reverse(path.begin(), path.end());
    task_out << path.size() << '\n' << path << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 msquare.cpp -o msquare.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address msquare.cpp -o msquare

Input:

4 3 1 2 5 6 7 8

Output:

22
ABBBCABBBCBBBCBCABCBBB

*/

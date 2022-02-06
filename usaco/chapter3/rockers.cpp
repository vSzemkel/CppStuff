/*
ID: marcin.8
LANG: C++
TASK: rockers
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=M8dKihAaeaL&S=rockers
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

std::ifstream task_in("rockers.in");
std::ofstream task_out("rockers.out");

int N, T, M; // num of songs, disk capacity, max number of disks
std::vector<int> items;
std::vector<bool> released;
std::vector<bool> dp; // dp[i] == bool(i is a sum of subset of first processed elements)
std::vector<std::vector<bool>> choosen; // choosen[i][j] == bool(j-th element is included in subset summing to i)

/**
 * @brief Selects unreleased songs to minimize disk space waste
 * @return int Number of songs
 */
static int subset_sum() {
    choosen.assign(T + 1, std::vector<bool>(N + 1));
    dp.assign(T + 1, false);
    dp[0] = true;
    for (int i = 0; i < N; ++i) {
        if (released[i])
            continue;
        for (int t = T; t >= items[i]; --t) {
            choosen[t][i + 1] = !dp[t] && dp[t - items[i]];
            dp[t] = dp[t] | dp[t - items[i]];
        }
    }

    int cap = -1;
    for (int z = T; z; --z)
        if (dp[z]) {
            cap = z;
            break;
        }

    if (~cap) {
        int acc{0};
        while (cap) {
            const auto& chn = choosen[cap];
            const int song = std::find(chn.begin(), chn.end(), true) - chn.begin() - 1;
            released[song] = true;
            cap -= items[song];
            ++acc;
        }
        return acc;
    }

    return 0;
}

int main(int, char**)
{
    task_in >> N >> T >> M;
    released.assign(N, false);
    items.resize(N);
    for (auto& s : items)
        task_in >> s;
    std::sort(items.begin(), items.end());

    std::priority_queue<int> albums;
    while (true) {
        const int next_album = subset_sum();
        if (next_album == 0)
            break;
        albums.push(next_album);
    }

    int ret{0};
    for (int z = M; z; --z)
        if (!albums.empty()) {
            ret += albums.top();
            albums.pop();
        }

    task_out << ret << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 rockers.cpp -o rockers.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address rockers.cpp -o rockers

Input:


Output:


*/


#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
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
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <vector>

// Level Design
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb1b6/0000000000c47792

static void solve() {
    int N;
    std::cin >> N;
    std::vector<int> permut(N);
    for (auto& n : permut) {
        std::cin >> n;
        --n;
    }

    // prepare reverse index
    std::vector<int> rev_ind(N);
    for (int i = 0; i < N; ++i)
        rev_ind[permut[i]] = i;

    // identify cycles
    std::vector<bool> visited(N);
    std::vector<int> cycles;
    for (int i = 0; i < N; ++i) {
        if (visited[i])
            continue;
        visited[i] = true;
        cycles.push_back(1);
        auto& cycle = cycles.back();
        auto next = rev_ind[i];
        while (!visited[next]) {
            visited[next] = true;
            ++cycle;
            next = rev_ind[next];
        }
    }

    std::sort(cycles.begin(), cycles.end());
    for (int level = 1; level <= N; ++level) {
        int ans = 1e09;
        const auto it = std::lower_bound(cycles.begin(), cycles.end(), level);
        if (it != cycles.end())
            ans = *it - level;
        if (ans > 1) {
            int count{0}, sum{0};
            int pos = it - cycles.begin() - 1;
            while (~pos && sum < level) {
                sum += cycles[pos];
                ++count;
                --pos;
            }
            if (sum >= level)
                ans = std::min(ans, count - 1 + sum - level);
        }

        std::cout << ans << ' ';
    }
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 level_design.cpp -o level_design.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address level_design.cpp -o level_design

Run:
level_design.exe < level_design.in

Input:

2
3
1 2 3
4
4 2 1 3

Output:


*/

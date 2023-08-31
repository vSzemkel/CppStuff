
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

// Frog Jump
// https://leetcode.com/problems/frog-jump/

static bool solve(const std::vector<int>& stones) {
    if (stones[1] != 1)
        return false;

    const auto N = int(stones.size());
    std::unordered_map<int, int> stones_revindex; // {distance -> stone index}
    for (int i = 0; i < N; ++i)
        stones_revindex[stones[i]] = i;
    std::vector<std::unordered_set<int>> jumps(N); // jumps[k] contains lengths of the last jump taking the frog to kth stone

    jumps[1].insert(1);
    for (int i = 1; i < N; i++)
        for (const int j : jumps[i])
            for (int probe = -1; probe <= 1; ++probe) {
                const auto destination = stones[i] + j + probe;
                if (i < destination && stones_revindex.contains(destination)) 
                    jumps[stones_revindex[destination]].insert(j + probe);
            }

    return !jumps[N - 1].empty();
}

static void io_handler() {
    int N;
    std::cin >> N;
    assert(1 < N);
    std::vector<int> stones(N);
    for (auto& s : stones)
        std::cin >> s;

    if (!solve(stones))
        std::cout << "IM";
    std::cout << "POSSIBLE";
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; io_handler(); std::cout << '\n';
    }
}

/*

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 frog_jump.cpp -o frog_jump.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address frog_jump.cpp -o frog_jump

Run:
frog_jump.exe < frog_jump.in

Input:


Output:


*/

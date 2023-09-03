
#include <cassert>
#include <iostream>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Frog Jump
// https://leetcode.com/problems/frog-jump/

/**
 * @brief Constant of 2 below is good enough for test suite on Leetcode
 * It is wrong in general. The only value that can be proved is n * (n + 1) / 2
 */
static bool solve_incorrect(const std::vector<int>& stones) {
    const auto N = int(stones.size());
    std::unordered_map<int, int> stones_revindex; // {distance -> stone index}
    std::function<bool(int, int, bool&)> solve_inner = [&](int current_stone, int last_jump, bool& dead_end) {
        if (current_stone == N - 1)
            return true;
        const auto current_distance = stones[current_stone];
        if (current_distance + last_jump + 2 < stones[current_stone + 1]) { // +2 is the tricky part in this solution
            dead_end = true;
            return false;
        }

        for (const int probe : {1, 0, -1}) {
            const auto delta = last_jump + probe;
            if (delta > 0) {
                const auto destination = current_distance + delta;
                const auto found = stones_revindex.find(destination);
                if (found != stones_revindex.end()) {
                    if (solve_inner(found->second, delta, dead_end))
                        return true;
                    if (dead_end)
                        return false;
                }
            }
        }

        return false;
    };

    stones_revindex.reserve(N);
    for (int ind = -1; const auto& s : stones)
        stones_revindex[s] = ++ind;

    bool dead_end{};
    return solve_inner(0, 0, dead_end);
}

static bool solve(const std::vector<int>& stones) {
    if (stones[1] != 1)
        return false;

    const auto N = int(stones.size());
    std::unordered_map<int, int> stones_revindex; // {distance -> stone index}
    stones_revindex.reserve(N);
    for (int ind = -1; const auto& s : stones)
        stones_revindex[s] = ++ind;
    std::vector<std::unordered_set<int>> jumps(N); // jumps[k] contains lengths of the last jump taking the frog to kth stone

    jumps[1].insert(1);
    const int final = stones.back();
    for (int i = 1; i < N - 1; ++i) {
        const auto& from_destination = stones[i];
        for (const int j : jumps[i])
            for (const int probe : {-1, 0, 1}) {
                const auto delta = j + probe;
                const auto destination = from_destination + delta;
                const auto found = stones_revindex.find(destination);
                if (found != stones_revindex.end()) {
                    if (destination == final)
                        return true;
                    jumps[found->second].insert(delta);
                }
            }
    }

    return N == 2;
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

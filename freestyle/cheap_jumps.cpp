
#include <cassert>
#include <iostream>
#include <deque>
#include <queue>
#include <tuple>
#include <vector>

// Cheap Jumps
// Given an array of size N of non negative integrals, you are to find
// a jump sequence from index 0 to N-1 with minimal sum of visited slots
// Single jump has length in range from 1 to J

using jump_t = std::tuple<int64_t, size_t, size_t>; // <cost in to, from, to>

size_t N, J;
std::vector<int64_t> input;

/**
 * @brief Jump to the current position from rmq result of previous at most J slots
 * 
 * @return Cost of the traversal
 */
auto solve_rmq() {
    std::deque<jump_t> dq;
    std::vector<size_t> prev(N);

    int64_t ret = input.back();
    if (N <= J)
        return ret;
    for (size_t i = 0; i < J; ++i) {
        while (!dq.empty() && input[i] < std::get<0>(dq.back()))
            dq.pop_back();
        dq.emplace_back(input[i], static_cast<size_t>(-1), i);
    }
    for (size_t i = 0; i < N; ++i) {
        if (!dq.empty() && std::get<1>(dq.front()) < i - J)
            dq.pop_front();
        const auto [cost, from, to] = dq.front();
        ret += cost;
        prev[to] = from;
        while (!dq.empty() && input[i] < std::get<0>(dq.back()))
            dq.pop_back();
        dq.emplace_back(input[i], to, i);
    }
    prev[N - 1] = std::get<2>(dq.front());

    size_t slot = N - 1;
    std::vector<size_t> path;
    while (~slot) {
        path.push_back(slot);
        slot = prev[slot];
    }

    std::cout << ret << "\nSTART_RMQ -> ";
    for (auto it = path.rbegin(); it != path.rend(); ++it)
        std::cout << (*it) + 1 << " -> ";
    std::cout << "END\n";

    return ret;
}

auto solve_mst() {
    std::vector<size_t> prev(N);
    std::vector<bool> visited(N);
    std::priority_queue<jump_t, std::vector<jump_t>, std::greater<>> pq;
    for (size_t i = 0; i < J && i < N; ++i)
        pq.emplace(input[i], -1, i);
    while (true) {
        const auto [_, from, to] = pq.top();
        pq.pop();
        prev[to] = from;
        visited[to] = true;
        if (to == N - 1) break;
        for (size_t i = 1; i <= J; ++i) {
            const auto next = to + i;
            if (next < N && !visited[next])
                pq.push(jump_t{input[next], to, next});
        }
    }

    int64_t ret{0};
    size_t slot = N - 1;
    std::vector<size_t> path;
    while (~slot) {
        ret += input[slot];
        path.push_back(slot);
        slot = prev[slot];
    }

    std::cout << ret << "\nSTART_MST -> ";
    for (auto it = path.rbegin(); it != path.rend(); ++it)
        std::cout << (*it) + 1 << " -> ";
    std::cout << "END\n";

    return ret;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cin >> N >> J;
        input.resize(N);
        for (auto& s : input)
            std::cin >> s;

        std::cout << "Case #" << g << ": ";
        const auto ret_mst = solve_mst();
        const auto ret_rmq = solve_rmq();
        //assert(ret_mst == ret_rmq);
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 cheap_jumps.cpp -o cheap_jumps.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 cheap_jumps.cpp -o cheap_jumps.o

Run:
cheap_jumps.exe < cheap_jumps.in

Input:

3
10 3
1 200 7 162 381 9 76 1 90 7
8 6
1 2 666 666 666 666 666 1
20 4
28 8 16 50 127 0 8 87 1111 876 9 12 7 32 918365896 8 1 90 37 5

Output:

Case #1: 25
0 -> 2 -> 5 -> 7 -> 9 -> END
Case #2: 4
0 -> 1 -> 7 -> END
Case #3: 66
0 -> 1 -> 5 -> 6 -> 10 -> 12 -> 16 -> 19 -> END

*/


#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

// Cheap Jumps
// Given an array of size N of non negative integrals, you are to find
// a jump sequence from index 0 to N-1 with minimal sum of visited slots
// Single jump has length in range from 1 to J

using jump_t = std::tuple<uint64_t, size_t, size_t>; // <weight, from, to>

void solve() {
    size_t N, J; std::cin >> N >> J;
    std::vector<uint64_t> input(N);
    for (auto& s : input)
        std::cin >> s;

    std::vector<size_t> prev(N);
    std::vector<bool> visited(N);
    std::priority_queue<jump_t, std::vector<jump_t>, std::greater<>> pq;
    for (size_t i = 1; i <= J && i < N; ++i)
        pq.push(jump_t{input[i], 0, i});
    visited[0] = true;
    while (true) {
        const jump_t jump = pq.top();
        const size_t from = std::get<1>(jump);
        const size_t to = std::get<2>(jump);
        pq.pop();
        if (visited[to] == true) continue;
        prev[to] = from;
        if (to == N - 1) break;
        for (size_t i = 1; i <= J; ++i) {
            const auto next = to + i;
            if (next < N) 
                pq.push(jump_t{input[next], to, next});
        }
    }

    size_t slot = N - 1;
    int64_t ret = input[0];
    std::vector<size_t> path;
    while (slot != 0) {
        ret += input[slot];
        path.push_back(slot);
        slot = prev[slot];
    }


    std::cout << ret << "\n0 -> ";
    for (auto it = path.rbegin(); it != path.rend(); ++it)
        std::cout << *it << " -> ";
    std::cout << "END\n";
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve();
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

C:\GitHub\GCC-test>cheap_jumps.exe < cheap_jumps.in
Case #1: 25
0 -> 2 -> 5 -> 7 -> 9 -> END
Case #2: 4
0 -> 1 -> 7 -> END
Case #3: 66
0 -> 1 -> 5 -> 6 -> 10 -> 12 -> 16 -> 19 -> END

*/

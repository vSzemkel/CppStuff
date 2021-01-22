
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>

// Security Update
// https://codingcompetitions.withgoogle.com/codejam/round/000000000019ffb9/000000000033871f

struct edge_t
{
    int c1;
    int c2;
};

void solve() {
    int C, D; std::cin >> C >> D;

    std::vector<edge_t> latencies(D);
    std::vector<std::pair<int, int>> orders, times(1); // {indicator, index}
    for (int i = 1; i < C; ++i) {
        int ind; std::cin >> ind;
        if (ind > 0) times.emplace_back(ind, i);
        else orders.emplace_back(-ind, i);
    }
    for (int i = 0; i < D; ++i) {
        int c1, c2; std::cin >> c1 >> c2; --c1; --c2;
        latencies[i].c1 = c1;
        latencies[i].c2 = c2;
    }

    std::sort(times.begin(), times.end());
    std::sort(orders.begin(), orders.end());
    for (auto& ord : orders) {
        assert(ord.first <= (int)times.size());
        const std::pair<int, int> t{times[ord.first - 1].first + 1, ord.second};
        const auto it = std::lower_bound(times.begin(), times.end(), t);
        times.insert(it, t);
    }

    std::vector<int> computer_times(C);
    for (const auto& t : times)
        computer_times[t.second] = t.first;

    for (const auto& e : latencies)
        std::cout << std::max(1, std::abs(computer_times[e.c1] - computer_times[e.c2])) << " ";
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << "\n";
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 security_update.cpp -o security_update.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 security_update.cpp -o security_update.o

Run:
security_update.exe < security_update.in

Input:

3
4 4
-1 -3 -2
1 2
1 3
2 4
3 4
4 4
-1 -1 -1
1 4
1 2
1 3
2 3
3 2
-2 -1
2 3
1 3

Output:

Case #1: 1 3 1 1 
Case #2: 1 1 1 1
Case #3: 1 1

*/

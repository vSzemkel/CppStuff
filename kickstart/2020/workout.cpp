
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <queue>
#include <vector>

// Workout
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ffc7/00000000001d3f5b#problem

static void solve() {
    int N, K, prev, max_diff{0};
    std::cin >> N >> K >> prev;
    std::vector<int> diffs(N - 1);
    for (int i = 0; i < N - 1; ++i) {
        int cur; std::cin >> cur;
        const auto d = cur - prev;
        max_diff = std::max(max_diff, d);
        diffs[i] = d;
        prev = cur;
    }

    const auto too_small = [&](const int64_t target) noexcept {
        int cost{0};
        for (const auto d : diffs) {
            if (d <= target) continue;
            cost += (d - 1) / target; // ceil(d, target) - 1 == (d + target - 1) / target - 1
            if (cost > K) return true;
        }
        return false;
    };

    int lo = 1, hi = max_diff + 1;
    while (lo < hi) {
        const auto mi = lo + (hi - lo) / 2;
        if (too_small(mi))
            lo = mi + 1;
        else 
            hi = mi;
    }

    std::cout << lo;
}

static void wrong() {
    int N, K; std::cin >> N >> K;
    int prev; std::cin >> prev;
    std::priority_queue<int64_t> pq;
    for (int i = 1; i < N; ++i) {
        int cur; std::cin >> cur;
        pq.push(cur - prev);
        prev = cur;
    }

    for (int i = 0; i < K; ++i) {
        const auto d = pq.top();
        if (d == 1) break;
        pq.pop();
        const auto d1 = d / 2;
        pq.push(d1); // THIS IS WRONG, SPLITTING IN HALVE IS SUBOPTIMAL
        pq.push(d - d1);
    }

    std::cout << pq.top();
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 -fsanitize=address workout.cpp -o workout.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address workout.cpp -o workout.o

Run:
py.exe interactive_runner.py py.exe workout_testing_tool.py 1 -- workout.exe
workout.exe < workout.in

Input:

3
5 2
10 13 15 16 17
5 6
9 10 20 26 30
8 3
1 2 3 4 5 6 7 10

Output:

Case #1: 2
Case #2: 3
Case #3: 1

*/

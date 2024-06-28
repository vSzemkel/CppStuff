
#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

// Magnetic Force Between Two Balls
// https://leetcode.com/problems/magnetic-force-between-two-balls

template <typename T, typename U>
static T last_true(T lo, T hi, U f) {
    lo--;
    assert(lo <= hi); // assuming f is decreasing
    while (lo < hi) { // find last index such that f is true
        const T mid = lo + (hi - lo + 1) / 2; // this will work for negative numbers too
        f(mid) ? lo = mid : hi = mid - 1;
    }
    return lo;
}

int maxDistance(std::vector<int>& position, int m)
{
    const int sz = int(position.size());
    std::sort(position.begin(), position.end());
    const auto check = [&](int candidate) {
        int cnt = 1, prev = position[0];
        for (int i = 1; i < sz; ++i)
            if (candidate <= position[i] - prev) {
                prev = position[i];
                ++cnt;
            }

        return m <= cnt; // can find at least m baskets with minimal distance on candidate
    };

    return last_true(1, position.back() - position.front(), check);
}

static void io_handler()
{
    int M, N;
    std::cin >> N >> M;
    std::vector<int> positions(N);
    for (auto& i : positions) std::cin >> i;

    std::cout << maxDistance(positions, M);
}

int main(int, char**)
{
    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": ";
        io_handler();
        std::cout << '\n';
    }
}

/*

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 magnetic_force_between_two_balls.cpp -o magnetic_force_between_two_balls.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address magnetic_force_between_two_balls.cpp -o magnetic_force_between_two_balls

Run:
magnetic_force_between_two_balls.exe < magnetic_force_between_two_balls.in

Input:

3
5 3
1 2 3 4 7
6 2
5 4 3 2 1 1000000000
5 6
1 2 3 4 5

Output:

Case #1: 3
Case #2: 999999999
Case #3: 0

*/

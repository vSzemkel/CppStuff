
#include <iostream>
#include <utility>
#include <vector>

// Latest Guests
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000051061/0000000000161427

/**
 * @brief Assume clockwise guests are moving in the right direction (we are in the center)
 */
static void solve() { // koodos to wifi
    int N, G, M;
    std::cin >> N >> G >> M;
    std::vector<int> left(N, -1), right(N, -1); // directional time of last visit
    std::vector<std::pair<int, bool>> guests(G); // {position, clockwise}
    for (auto& g : guests) {
        char dir;
        std::cin >> g.first >> dir;
        --g.first;
        g.second = dir == 'C';
        if (g.second)
            right[(g.first + M) % N] = M; // after M minutes clockwise guests reach here
        else
            left[(g.first - (M % N) + N) % N] = M;
    }

    // compute directional time of the last visit for every consulate
    int cur = -1;
    for (int i = 0; i < 2 * N; ++i) {
        cur = std::max(cur - 1, left[i % N]);
        left[i % N] = cur;
    }
    cur = -1;
    for (int i = 2 * N - 1; ~i; --i) {
        cur = std::max(cur - 1, right[i % N]);
        right[i % N] = cur;
    }

    // for every consulate score last visiting guest
    std::vector<int> lcnt(N, 0), rcnt(N, 0);
    for (int i = 0; i < N; ++i) {
        const int dir = std::max(left[i], right[i]);
        if (dir < 0) continue;
        if (dir == left[i]) {
            const int origin = (i + dir) % N;
            ++lcnt[origin];
        }
        if (dir == right[i]) {
            const int origin = (i - (dir % N) + N) % N;
            ++rcnt[origin];
        }
    }

    for (auto& [pos, dir] : guests)
        std::cout << (dir ? rcnt : lcnt)[pos] << ' ';
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 latest_guests.cpp -o latest_guests.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address latest_guests.cpp -o latest_guests

Run:
latest_guests.exe < latest_guests.in

Input:

4
5 3 2
5 C
2 A
1 A
2 4 0
1 A
1 C
1 A
1 C
3 2 10
3 C
2 A
6 1 6
4 A

Output:

Case #1: 2 2 1 
Case #2: 1 1 1 1
Case #3: 2 2
Case #4: 6

*/


#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

// Scheduling a Meeting
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb409/0000000000bef943

/**
 * @brief For every time window compute number of meetings each manager has
 * Sort by availability and choose K least occupied managers for every
 * possible X long meeting timing
 */
static void solve_set1() { // TLE: O(D * NlogN)
    int N, K, X, D, M;
    std::cin >> N >> K >> X >> D >> M;

    std::vector<int> meetings(N, 0); // number of meetings a manager has in current window
    std::vector<std::vector<int>> increase_events(D + 1), decrease_events(D + 1);
    for (int z = M; z; --z) {
        int m, b, e;
        std::cin >> m >> b >> e; // [b, e)
        --m;
        if (b < X)
            ++meetings[m];
        else
            increase_events[b - X + 1].push_back(m);

        decrease_events[e].push_back(m);
    }

    int64_t ans = 1e09;
    for (int i = 0; i <= D - X; ++i) {
        for (const int j : increase_events[i])
            ++meetings[j];
        for (const int j : decrease_events[i])
            --meetings[j];

        auto copy = meetings;
        std::partial_sort(copy.begin(), copy.begin() + K, copy.end());
        ans = std::min(ans, std::accumulate(copy.begin(), copy.begin() + K, int64_t{}));
    }

    std::cout << ans;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve_set1(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 scheduling_a_meeting.cpp -o scheduling_a_meeting.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address scheduling_a_meeting.cpp -o scheduling_a_meeting

Run:
scheduling_a_meeting.exe < scheduling_a_meeting.in

Input:

3
3 2 2 6
5
1 3 5
2 1 3
2 2 6
3 0 1
3 3 6
3 3 2 6
5
1 3 5
2 1 3
2 2 6
3 0 1
3 3 6
3 2 3 6
5
1 3 5
2 1 3
2 2 6
3 0 1
3 3 6

Output:

Case #1: 0
Case #2: 2
Case #3: 1

*/


#include <algorithm>
#include <iostream>
#include <numeric>
#include <set>
#include <utility>
#include <vector>

// Scheduling a Meeting
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb409/0000000000bef943

/**
 * @brief Rebalance two sets of managers of size K and N - K
 * K set contains managers with K-lowest number of meetings
 */
static void solve() { // by Geothermal O(MlogN)
    int N, K, X, D, M;
    std::cin >> N >> K >> X >> D >> M;

    std::vector<int> meetings(N); // number of meetings a manager has in current window
    std::vector<std::pair<int, std::pair<int, int>>> events; // pos -> {manager, delta}
    for (int z = M; z; --z) {
        int m, b, e;
        std::cin >> m >> b >> e; // [b, e)
        --m;
        events.emplace_back(b + 1, std::pair{m, +1});
        events.emplace_back(e + X, std::pair{m, -1});
    }

    std::sort(events.begin(), events.end());

    int64_t totalIn{0};
    std::set<std::pair<int, int>> inSet, outSet;
    for (int i = 0; i < K; ++i)
        inSet.emplace(0, i);
    for (int i = K; i < N; ++i)
        outSet.emplace(0, i);

    int64_t pos{0}, ans{M};
    for (int i = 0; i <= D; ++i) {
        while (pos < 2 * M && events[pos].first == i) {
            const auto [manager, delta] = events[pos].second;
            const std::pair<int, int> cur = {meetings[manager], manager};
            if (inSet.count(cur)) {
                inSet.erase(cur);
                meetings[manager] += delta;
                inSet.emplace(meetings[manager], manager);
                totalIn += delta;
            } else {
                outSet.erase(cur);
                meetings[manager] += delta;
                outSet.emplace(meetings[manager], manager);
            }
            ++pos;
        }

        while (!inSet.empty() && !outSet.empty() && inSet.rbegin()->first > outSet.begin()->first) {
            const auto toIn = *outSet.begin();
            const auto toOut = *inSet.rbegin();
            totalIn -= toOut.first - toIn.first;
            outSet.insert(inSet.extract(toOut));
            inSet.insert(outSet.extract(toIn));
        }

        if (i >= X)
            ans = std::min(ans, totalIn);
    }

    std::cout << ans;
}

/**
 * @brief For every time window compute number of meetings each manager has
 * Sort by availability and choose K least occupied managers for every
 * possible X long meeting timing
 */
static void solve_set1() { // TLE: O(D * NlogN)
    int N, K, X, D, M;
    std::cin >> N >> K >> X >> D >> M;

    std::vector<int> meetings(N); // number of meetings a manager has in current window
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

    int64_t ans = M;
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
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
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

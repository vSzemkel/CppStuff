
#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

// Story of Seasons
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb409/0000000000bef319

struct seed_t {
    int Q, V;
    int64_t L;

    bool operator<(const seed_t &other) const {
        return L < other.L;
    }
};

/**
 * Observation1: Each day plant maximal accessible number of seeds
 * Observation2: Seeds of the same type are indistinguishable
 * Observation2: Try to plant more valuable before less valuable
 * Observation3: Try to plan longer lifetimers before shorter
 * Observation4: Every seed type has a deadline. For each day consider
 * only types within the deadline. Then plant most valuable first.
 * Trick1: Assume only one seed can be planted each day
 */
static void solve() { // by neal_wu
    int64_t D, N, X;
    std::cin >> D >> N >> X;
    std::vector<seed_t> seeds(N);
    for (auto& s : seeds)
        std::cin >> s.Q >> s.L >> s.V;

    std::sort(seeds.begin(), seeds.end());

    std::priority_queue<std::pair<int, int64_t>> pq; // {value, slots}

    int64_t ans{0};
    for (int i = 0; i < N; ++i) {
        pq.emplace(seeds[i].V, seeds[i].Q);
        const int64_t overlap_start = i < N - 1 ? seeds[i + 1].L : D;
        int64_t slots = (overlap_start - seeds[i].L) * X;

        while (slots > 0 && !pq.empty()) {
            std::pair<int, int64_t> top = pq.top(); pq.pop();
            const int64_t take = std::min(slots, top.second);
            top.second -= take;
            slots -= take;
            ans += take * top.first;

            if (top.second > 0)
                pq.push(top);
        }
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 story_of_seasons.cpp -o story_of_seasons.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address story_of_seasons.cpp -o story_of_seasons

Run:
story_of_seasons.exe < story_of_seasons.in

Input:

1
5 3 4
5 2 3
2 3 10
2 4 5

Output:


*/

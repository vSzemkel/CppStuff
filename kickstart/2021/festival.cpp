
#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

// Festival
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000435bae/0000000000887dba

struct event_t {
    int64_t start;
    int64_t finish;
    int64_t weight;
    bool operator<(const event_t& other) { return start < other.start; }   // for sort
    bool operator>(const event_t& other) { return finish > other.finish; } // for pq
};

static void solve() {
    int64_t D, N, K, ret{0};
    std::cin >> D >> N >> K;
    std::vector<event_t> sections(N);
    for (auto& s : sections)
        std::cin >> s.weight >> s.start >> s.finish;
    std::sort(sections.begin(), sections.end());

    int64_t sum{0};
    std::vector<int64_t> happy; // implement std::multiset
    std::priority_queue<event_t, std::vector<event_t>, std::greater<>> pq; // {end, happy}
    for (const auto& s : sections) {
        while (!pq.empty() && pq.top().finish < s.start) {
            const auto it = std::lower_bound(happy.begin(), happy.end(), pq.top().weight);
            const auto next_h = happy.erase(it);
            if (happy.end() - next_h < K) {
                sum -= pq.top().weight;
                const auto size = int64_t(happy.size());
                if (size >= K)
                    sum += happy[size - K];
            }
            pq.pop();
        }

        const auto& hp = s.weight;
        const auto it = std::lower_bound(happy.begin(), happy.end(), hp);
        const auto new_h = happy.insert(it, hp);
        if (happy.end() - new_h <= K) {
            sum += hp;
            const auto size = int64_t(happy.size());
            if (size > K)
                sum -= happy[size - K - 1];
        }

        ret = std::max(ret, sum);

        pq.push(s);
    }

    std::cout << ret;
}

static void solve_with_multiset() { // 15x faster
    int64_t D, N, K, ret{0};
    std::cin >> D >> N >> K;
    std::vector<event_t> sections(N);
    for (auto& s : sections)
        std::cin >> s.weight >> s.start >> s.finish;
    std::sort(sections.begin(), sections.end());

    int64_t sum_hi{0};
    std::multiset<int64_t> lo, hi;
    std::priority_queue<event_t, std::vector<event_t>, std::greater<>> pq; // {end, happy}
    for (const auto& s : sections) {
        while (!pq.empty() && pq.top().finish < s.start) {
            const auto& weight = pq.top().weight;
            auto it = lo.find(weight);
            if (it != lo.end())
                lo.erase(it);
            else {
                it = hi.find(weight);
                hi.erase(it);
                sum_hi -= weight;
            }
            pq.pop();
        }
        while (int64_t(hi.size()) < K && !lo.empty()) {
            const auto last = std::prev(lo.end());
            const auto val = *last;
            lo.erase(last);
            hi.insert(val);
            sum_hi += val;
        }

        const auto& hp = s.weight;
        if (int64_t(hi.size()) == K && hp < *hi.begin())
            lo.insert(hp);
        else {
            sum_hi += hp;
            hi.insert(hp);
            if (int64_t(hi.size()) > K) {
                const auto first = hi.begin();
                int val = *first;
                hi.erase(first);
                lo.insert(val);
                sum_hi -= val;
            }
        }

        ret = std::max(ret, sum_hi);

        pq.push(s);
    }

    std::cout << ret;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        //std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
        std::cout << "Case #" << g << ": "; solve_with_multiset(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 festival.cpp -o festival.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address festival.cpp -o festival

Run:
festival.exe < festival.in

Input:

2
10 4 2
800 2 8
1500 6 9
200 4 7
400 3 5
5 3 3
400 1 3
500 5 5
300 2 3

Output:

Case #1: 2300
Case #2: 700

*/

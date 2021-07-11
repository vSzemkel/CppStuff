
#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

// Cutting Intervals
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000004361e3/000000000082b933

struct segment_t {
    segment_t(const int64_t s) : start_pos(s) {};
    int64_t start_pos;
    int start_depth{0};
    int segment_depth{0};
    friend bool operator<(const segment_t& lhs, const segment_t& rhs){ return lhs.start_pos < rhs.start_pos; }
    friend bool operator==(const segment_t& lhs, const segment_t& rhs){ return lhs.start_pos == rhs.start_pos; }
};

static void solve() {
    int N; int64_t C; std::cin >> N >> C;
    std::vector<std::array<int64_t, 2>> segments(N);
    for (auto& s : segments)
        std::cin >> s[0] >> s[1];
    std::sort(segments.begin(), segments.end());
    std::vector<segment_t> points;
    points.reserve(2 * N);
    for (auto& s : segments) {
        points.emplace_back(s[0]);
        points.emplace_back(s[1]);
    }
    std::sort(points.begin(), points.end());
    const auto it = std::unique(points.begin(), points.end());
    points.erase(it, points.end());
    // initialize points
    for (const auto& s : segments) {
        const auto start = std::lower_bound(points.begin(), points.end(), segment_t{s[0]});
        auto it = start;
        while (it != points.end() && it->start_pos < s[1]) {
            ++it->segment_depth;
            ++it;
        }
        it = start + 1;
        while (it != points.end() && it->start_pos < s[1]) {
            ++it->start_depth;
            ++it;
        }
    }
    // load priority queue
    std::priority_queue<std::pair<int, int64_t>> cuts; // {depth, count}
    for (int i = 1; i < int(points.size()); ++i) {
        cuts.push({points[i - 1].start_depth, 1});
        const auto cnt = points[i].start_pos - points[i - 1].start_pos - 1;
        if (cnt > 0)
            cuts.push({points[i - 1].segment_depth, cnt});
    }
    // compute result
    int64_t ret{N};
    while (!cuts.empty() && C > 0) {
        const auto& c = cuts.top();
        const int64_t count = std::min(c.second, C);
        ret += c.first * count;
        cuts.pop();
        C -= count;
    }

    std::cout << ret;
}

static void solve_set1() {
    int N; int64_t C; std::cin >> N >> C;
    std::array<int64_t, 2> s;
    std::unordered_map<int64_t, int> scores;
    for (int i = 0; i < N; ++i) {
        std::cin >> s[0] >> s[1];
        for (auto i = s[0] + 1; i < s[1]; ++i)
            ++scores[i];
    }

    std::vector<int> cuts;
    cuts.resize(scores.size());
    for (const auto& s : scores)
        cuts.push_back(s.second);
    std::sort(cuts.begin(), cuts.end(), [](const auto n1, const auto n2){ 
        return n1 > n2;
    });

    int64_t ret = std::accumulate(cuts.begin(), cuts.begin() + std::min(int64_t(cuts.size()), C), 0LL);
    std::cout << ret + N;
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 cutting_intervals.cpp -o cutting_intervals.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address cutting_intervals.cpp -o cutting_intervals

Run:
cutting_intervals.exe < cutting_intervals.in

Input:

1
3 3
1 3
2 4
1 4

Output:

Case #1: 7

*/

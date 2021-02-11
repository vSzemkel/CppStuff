
#include <algorithm>
#include <iostream>
#include <numeric>
#include <optional>
#include <utility>
#include <vector>

// CJ20R3T4: Recalculating
// https://codingcompetitions.withgoogle.com/codejam/round/000000000019ff7e/00000000003775e9


using point_t = std::pair<int64_t, int64_t>;
using section_t = std::pair<int64_t, int64_t>;

int64_t g_D;

point_t rotate45scale(const point_t& p)
{
    // x1 = x * cos - y * sin
    // y1 = x * sin + y * cos
    // sin(-45) < 0, cos(-45) > 0
    // rotate by -45 and enlarge by factor sqrt(2)
    return {p.first + p.second, p.second - p.first};
}

bool strong_overlaping(const point_t& s1, const point_t& s2)
{
    return std::abs(s2.first - s1.first) < g_D && std::abs(s2.second - s1.second) < g_D;
}

std::optional<section_t> find_intersection1d(section_t s1, section_t s2, bool normalize = true)
{
    if (normalize) {
        if (s1.first > s2.first)
            std::swap(s1, s2);
        if (s1.first > s1.second)
            std::swap(s1.first, s1.second);
        if (s2.first > s2.second)
            std::swap(s2.first, s2.second);
    }

    if (s1.second > s2.first)
        return {{std::max(s1.first, s2.first), std::min(s1.second, s2.second)}};

    return std::nullopt;
}

int64_t intersection_area(const point_t& s1, const point_t& s2)
{
    const auto dx = find_intersection1d({s1.first - g_D, s1.first + g_D}, {s2.first - g_D, s2.first + g_D});
    const auto dy = find_intersection1d({s1.second - g_D, s1.second + g_D}, {s2.second - g_D, s2.second + g_D});
    if (!dx || !dy) return 0;
    return (dx.value().second - dx.value().first) * (dy.value().second - dy.value().first);
}

int64_t union_area(const point_t& s1, const point_t& s2)
{
    const auto dx = 2 * g_D + std::abs(s2.first - s1.first);
    const auto dy = 2 * g_D + std::abs(s2.second - s1.second);
    return dx * dy;
}

void solve() {
    int N; std::cin >> N >> g_D;
    std::vector<point_t> points(N);
    for (int i = 0; i < N; ++i)
        std::cin >> points[i].first >> points[i].second;

    std::transform(points.begin(), points.end(), points.begin(), rotate45scale);
    const auto ia = intersection_area(points[0], points[1]);
    const auto square = 4 * g_D * g_D;
    const auto total = 2 * square - ia;
    int64_t distinguishable{0};
    if (ia > 0) {
        if (strong_overlaping(points[0], points[1])) {
            const auto convex = union_area(points[0], points[1]);
            distinguishable = total - 2 * (convex - total);
        } else
            distinguishable = 3 * ia;
    }
    const auto cd = std::gcd(distinguishable, total);
    std::cout << distinguishable / cd << " " << total / cd << "\n";
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
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 recalculating.cpp -o recalculating.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 recalculating.cpp -o recalculating.o

Run:
recalculating.exe < recalculating.in

Input:

5
2 4
0 0
0 1
2 4
0 0
5 0
2 1
0 0
5 0
2 4
0 0
4 4
2 4
0 0
5 1

Output:

Case #1: 75 79
Case #2: 27 119
Case #3: 0 1
Case #4: 0 1
Case #5: 1 5

*/

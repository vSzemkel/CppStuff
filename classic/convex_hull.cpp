
#include <cmath>
#include <iostream>
#include <queue>
#include <random>

// Convex Hull
// Classic algorithm implementation with random points 
// from fixed region and priority queue of polar angles
// For more effective algo see ./point.cpp

// case definition
constexpr int g_points_count = 100;

using point_t = std::pair<int, int>;
using hull_t = std::vector<point_t>;
using comp_t = bool(*)(const point_t&, const point_t&);
using queue_t = std::priority_queue<point_t, hull_t, comp_t>;
queue_t init_pq() 
{
    // polar angles comparer
    auto comp = [](const point_t& p1, const point_t& p2) { 
        const auto polar1 = atan2(p1.second, p1.first);
        const auto polar2 = atan2(p2.second, p2.first);
        return polar1 > polar2 || (polar1 == polar2 && sqrt(p1.first*p1.first + p1.second*p1.second) > sqrt(p2.first*p2.first + p2.second*p2.second));
    };
    // container with coordinates origin and random points
    hull_t tmp;
    tmp.reserve(g_points_count);
    tmp.emplace_back(0, 0);
    std::random_device rd;
    std::uniform_int_distribution<int> dist(-100, 100);
    for (int n = 1; n < g_points_count; ++n)
        tmp.emplace_back(dist(rd), dist(rd) + 101);

    return {std::move(comp), std::move(tmp)}; // linear make_heap underneath
}

// globals
queue_t g_points{init_pq()};
hull_t g_cvhull;

void print_pq() {
    while (!g_points.empty()) {
        const point_t& p = g_points.top();
        std::cout << "\t[" << p.first << ", " << p.second << "]\n";
        g_points.pop();
    }
    std::cout << '\n';
}

int are_counterclockwise(const point_t& a, const point_t& b, const point_t& c)
{
    auto area2 = (b.first - a.first) * (c.second - a.second) - (b.second - a.second) * (c.first - a.first);
    if (area2 < 0) return -1; // clockwise
    if (area2 > 0) return 1;  // counter-clockwise
    return 0;                 // collinear
}

int main(int argc, char* argv[])
{
    static_assert(g_points_count > 2);
    if (argc > 1) {
        print_pq();
        return 0;
    }

    g_cvhull.push_back(g_points.top());
    g_points.pop();
    g_cvhull.push_back(g_points.top());
    g_points.pop();
    while (!g_points.empty()) {
        const point_t& next = g_points.top();
        if (next != g_cvhull.back()) {
            while (are_counterclockwise(g_cvhull[g_cvhull.size() - 2], g_cvhull.back(), next) <= 0)
                g_cvhull.pop_back();
            g_cvhull.push_back(next);
        }
        g_points.pop();
    }

    // present result
    printf("\nRecognized convex hull of size %zu from %i points:\n", g_cvhull.size(), g_points_count);
    for (const auto& v : g_cvhull)
        printf("\t[%i, %i]\n", v.first, v.second);
}

/*    clang++.exe -Wall -g -std=c++17 convex_hull.cpp -o convex_hull.exe

Sample output:

Recognized convex hull of size 12 from 100 points:
        [0, 0]
        [94, 5]
        [98, 30]
        [93, 192]
        [91, 196]
        [36, 197]
        [5, 196]
        [-68, 192]
        [-94, 133]
        [-96, 125]
        [-100, 36]
        [-96, 3]

*/

#include <algorithm>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

// Star trappers
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000435bae/0000000000888d45


template <typename T = int64_t>
struct point_t {
    point_t(T _x = T{}, T _y = T{}) : x(_x), y(_y) {}
    T cross(const point_t& p) const { return x * p.y - y * p.x; }
    T cross(const point_t& a, const point_t& b) const { return (a - *this).cross(b - *this); }
    T dot(const point_t& p) const { return x * p.x + y * p.y; }
    T dot(const point_t& a, const point_t& b) const { return (a - *this).dot(b - *this); }
    T sqrLen() const { return dot(*this); }
    auto len() const { return std::sqrt(dot(*this)); }
    auto get_polar() const { return std::atan2(y, x); }
    point_t operator+(const point_t& p) const { return point_t{x + p.x, y + p.y}; }
    point_t operator-(const point_t& p) const { return point_t{x - p.x, y - p.y}; }
    bool operator==(const point_t& other) const { return x == other.x && y == other.y; }
    bool operator<(const point_t& other) const { return x < other.x || (x == other.x && y < other.y); }
    friend std::ostream& operator<<(std::ostream& out, const point_t& p) { return out << p.x << ' ' << p.y; }
    friend std::istream& operator>>(std::istream& in, point_t& p) { in >> p.x >> p.y; return in; }

    int turning_left(const point_t& b, const point_t& c) const { // consider path: this -> b -> c
        const auto area2 = (b.x - x) * (c.y - y) - (b.y - y) * (c.x - x);
        if (area2 < 0) return -1; // clockwise
        if (area2 > 0) return 1;  // counter-clockwise
        return 0;                 // colinear
    }

    bool in_triangle(const point_t& a, const point_t& b, const point_t& c) const { // may be on border
        const auto s1 = std::abs(a.cross(b, c));
        const auto s2 = std::abs(cross(a, b)) + std::abs(cross(b, c)) + std::abs(cross(c, a));
        return s1 == s2;
    }

    bool in_triangle_strict(const point_t& a, const point_t& b, const point_t& c) const { // may not be on border
        const auto result = a.turning_left(b, *this) + b.turning_left(c, *this) + c.turning_left(a, *this);
        return std::abs(result) == 3;
    }

    bool polar_cmp(const point_t& other) const { return get_polar() > other.get_polar(); };

    bool polar_radius_cmp(const point_t& other) const {
        const auto p1 = get_polar();
        const auto p2 = other.get_polar();
        return p1 > p2 || (p1 == p2 && sqrLen() < other.sqrLen());
    };

    T x, y;
};

template <typename T = int64_t>
static bool polar_cmp(const point_t<T>& p1, const point_t<T>& p2) { return p1.polar_cmp(p2); };
template <typename T = int64_t>
static bool polar_radius_cmp(const point_t<T>& p1, const point_t<T>& p2) { return p1.polar_radius_cmp(p2); };

constexpr const double EPS = 1e-9;
constexpr const double INF = 1e18;

int N;
double ret;
point_t<> blue;
std::vector<point_t<>> points;

static void finish1()
{
    // check all triangles
    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j)
            for (int k = j + 1; k < N; ++k) {
                auto& a = points[i];
                auto& b = points[j];
                auto& c = points[k];
                if (blue.in_triangle_strict(a, b, c)) {
                    const auto cur = (a - b).len() + (b - c).len() + (c - a).len();
                    ret = std::min(ret, cur);
                }
            }

    // check all 4-poligons with diagonals crossing at origin
    std::vector<std::array<int, 2>> pairs;
    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j)
            if (points[i].x * points[j].y == points[j].x * points[i].y)
                pairs.push_back({i, j});

    const int ps = int(pairs.size());
    for (int i = 0; i < ps; ++i)
        for (int j = i + 1; j < ps; ++j) {
            const auto cur = (points[pairs[i][0]] - points[pairs[j][0]]).len() + (points[pairs[i][0]] - points[pairs[j][1]]).len()
                           + (points[pairs[i][1]] - points[pairs[j][0]]).len() + (points[pairs[i][1]] - points[pairs[j][1]]).len();
            ret = std::min(ret, cur);
        }
}

static void finish2() // by Benq
{
    std::vector<std::vector<double>> dist(N, std::vector(N, INF));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (points[i].cross(points[j]) > 0)
                dist[i][j] = std::min(dist[i][j], (points[i] - points[j]).len());

    for (int k = 0; k < N; ++k)
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);

    for (int i = 0; i < N; ++i)
        ret = std::min(ret, dist[i][i]);
}

static void solve()
{
    std::cin >> N;
    points.resize(N);
    for (auto& p : points)
        std::cin >> p;
    point_t<> blue;
    std::cin >> blue;

    // deduplicate, translate, throw away farthest
    auto ne = std::remove(points.begin(), points.end(), blue);
    points.erase(ne, points.end());
    std::transform(points.begin(), points.end(), points.begin(), [&blue](const auto& p){
        return p - blue;
    });
    blue = {0, 0};
    std::sort(points.begin(), points.end(), polar_radius_cmp<>);
    double last_polar = points.back().get_polar();
    for (int i = N - 1; i > 0; --i) {
        auto prev_polar = points[i - 1].get_polar();
        if (std::abs(prev_polar - last_polar) < EPS) // prev_polar == last_polar will do here
            points[i].x = INF;
        last_polar = prev_polar;
    }
    ne = std::remove_if(points.begin(), points.end(), [](const auto& p){ return p.x == INF; });
    points.erase(ne, points.end());
    N = int(points.size());

    ret = INF;
    finish1();

    if (ret == INF)
        std::cout << "IMPOSSIBLE";
    else
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
        std::cout << "Case #" << g << ": " << std::fixed << std::setprecision(9); solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 star_trappers.cpp -o star_trappers.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address star_trappers.cpp -o star_trappers

Run:
star_trappers.exe < star_trappers.in

Input:

2
2
0 0
5 0
2 2
3
0 0
5 0
0 5
1 1

Output:

Case #1: IMPOSSIBLE
Case #2: 17.071068

*/

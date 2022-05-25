
#include <algorithm>
#include <array>
#include <assert.h>
#include <cmath>
#include <iostream>
#include <optional>
#include <vector>

// see: /kickstart/2021/star_trappers.cpp
// integral points in trangle, see: https://en.wikipedia.org/wiki/Pick's_theorem

constexpr const double EPS = 1e-9;
constexpr const double INF = 1e18;

template <typename T> struct line_t;

template <typename T = int64_t>
struct point_t {
    point_t(T _x = T{}, T _y = T{}) : x(_x), y(_y) {}
    int sgn(const T a) const { return (a > EPS) - (a < -EPS); }
    T cross(const point_t& p) const { return x * p.y - y * p.x; }
    T cross(const point_t& a, const point_t& b) const { return (a - *this).cross(b - *this); }
    T dot(const point_t& p) const { return x * p.x + y * p.y; }
    T dot(const point_t& a, const point_t& b) const { return (a - *this).dot(b - *this); }
    T sqrLen() const { return dot(*this); }
    auto len() const { return std::sqrt(dot(*this)); }
    auto polar() const { return std::atan2(y, x); }
    point_t unit() const { return *this / len(); }
    point_t conj() const { return {x, -y}; }
    point_t perp() const { return {-y, x}; }
    point_t dir(const double ang) const { return {std::cos(ang), std::sin(ang)}; }
    point_t rotate(const double ang) const { return { *this * point_t{std::cos(ang), std::sin(ang)}}; }
    point_t operator-() const { return point_t{-x, -y}; }
    point_t operator*(const T m) const { return point_t{x * m, y * m}; }
    point_t operator/(const T d) const { return point_t{x / d, y / d}; }
    point_t operator+(const point_t& p) const { return point_t{x + p.x, y + p.y}; }
    point_t operator-(const point_t& p) const { return point_t{x - p.x, y - p.y}; }
    point_t operator*(const point_t& p) const { return {x * p.x - y * p.y, y * p.x + x * p.y}; }
    point_t operator/(const point_t& p) const { return *this * p.conj() / p.sqrLen(); }
    bool operator==(const point_t& other) const { return x == other.x && y == other.y; }
    bool operator<(const point_t& other) const { return x < other.x || (x == other.x && y < other.y); }

    point_t reflect(const line_t<T>& l) const {
        auto& a = l[0]; auto delta = l[1] - l[0];
        return a + ((*this - a) / delta).conj() * delta;
    }

    point_t foot(const line_t<T>& l) const {
        return (*this + reflect(l)) / T{2};
    }

    bool on_segment(const line_t<T>& l) const {
        return sgn(cross(l[0], l[1])) == 0 && sgn((*this - l[0]).dot(*this - l[1])) <= 0; 
    }

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
        const auto p1 = polar();
        const auto p2 = other.polar();
        return p1 > p2 || (p1 == p2 && sqrLen() < other.sqrLen());
    };

    inline static T area(const point_t& a, const point_t& b, const point_t& c) { return std::abs(a.cross(b, c)) / 2; };

    friend std::ostream& operator<<(std::ostream& out, const point_t& p) { return out << p.x << ' ' << p.y; }
    friend std::istream& operator>>(std::istream& in, point_t& p) { in >> p.x >> p.y; return in; }

    T x, y;
};

template <typename T = int64_t>
struct line_t : std::array<point_t<T>, 2> {
    line_t(const T x0, const T y0, const T x1, const T y1) {
        (*this)[0] = {x0, y0};
        (*this)[1] = {x1, y1};
    }

    line_t(const point_t<T>& p0, const point_t<T>& p1) {
        (*this)[0] = p0;
        (*this)[1] = p1;
    }

    static bool intersects(const line_t<T>& s1, const line_t<T>& s2) {
        const auto d1 = s1[0].turning_left(s1[1], s2[0]);
        const auto d2 = s1[0].turning_left(s1[1], s2[1]);
        const auto d3 = s2[0].turning_left(s2[1], s1[0]);
        const auto d4 = s2[0].turning_left(s2[1], s1[1]);
        if (d1 * d2 < 0 && d3 * d4 < 0)
            return true;
        if (s1[0].on_segment(s2) || s1[1].on_segment(s2))
            return true;
        if (s2[0].on_segment(s1) || s2[1].on_segment(s1))
            return true;
        return false;
    }

    /**
     * @brief Finds intersection between two sections in general position (distinct points, non parallel sections)
     * adapted from: https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
     */
    static std::optional<point_t<T>> find_intersection(const line_t<T>& s1, const line_t<T>& s2) {
        const T s1_dx = s1[1].x - s1[0].x;
        const T s1_dy = s1[1].y - s1[0].y;
        const T s2_dx = s2[1].x - s2[0].x;
        const T s2_dy = s2[1].y - s2[0].y;

        const T s = (-s1_dy * (s1[0].x - s2[0].x) + s1_dx * (s1[0].y - s2[0].y)) / (-s2_dx * s1_dy + s1_dx * s2_dy);
        if (s < 0 || 1 < s)
            return std::nullopt;
        const T t = ( s2_dx * (s1[0].y - s2[0].y) - s2_dy * (s1[0].x - s2[0].x)) / (-s2_dx * s1_dy + s1_dx * s2_dy);
        if (t < 0 || 1 < t)
            return std::nullopt;

        return point_t<T>{s1[0].x + (t * s1_dx), s1[0].y + (t * s1_dy)};
    }
};

template <typename T = int64_t>
static bool polar_cmp(const point_t<T>& p1, const point_t<T>& p2) { return p1.polar_cmp(p2); };
template <typename T = int64_t>
static bool polar_radius_cmp(const point_t<T>& p1, const point_t<T>& p2) { return p1.polar_radius_cmp(p2); };
template <class T = double>
static T polygon_area2(const std::vector<point_t<T>>& v) { // doubled area for circuit ordered points
    T a = v.back().cross(v.front());
    for (int i = 0; i < int(v.size()) - 1; ++i)
        a += v[i].cross(v[i + 1]);
    return std::abs(a);
}

/**
 * @brief Jarvis's march
 */
template <typename T = int64_t>
static auto convex_hull(std::vector<point_t<T>> points) { // copy is sorted
    decltype(points) upper, lower;
    std::sort(points.begin(), points.end());
    for (const auto& p : points) {
        int usz = int(upper.size());
        while (usz >= 2 && p.turning_left(upper[usz - 1], upper[usz - 2]) == -1) {
            upper.pop_back();
            --usz;
        }
        upper.push_back(p);
        int lsz = int(lower.size());
        while (lsz >= 2 && lower[lsz - 2].turning_left(lower[lsz - 1], p) == -1) {
            lower.pop_back();
            --lsz;
        }
        lower.push_back(p);
    }

    std::reverse(upper.begin(), upper.end());
    lower.pop_back();
    lower.insert(lower.end(), upper.begin(), upper.end());
    lower.pop_back();

    return lower;
}

template <typename T = int64_t>
static auto perimeter_len(const std::vector<point_t<T>>& points) { // points are perimeter ordered
    T perm{0};
    for (int z = int(points.size()) - 1; z; --z)
        perm += (points[z] - points[z - 1]).len();
    perm += (points.back() - points.front()).len();
    return perm;
}

int main(int, char**)
{
    const point_t<> a{0, 0};
    const point_t<> b{4, 0};
    const point_t<> c{0, 4};
    const point_t<> d{2, 2};
    const point_t<> e{1, 1};
    const point_t<> f{3, 3};
    const point_t<> g{2, -1};
    const point_t<> h{-1, 3};

    assert(EPS < INF);
    assert(d.in_triangle(a, b, c));
    assert(e.in_triangle_strict(a, b, c));
    assert(!d.in_triangle_strict(a, b, c)); // on the border

    assert(!f.in_triangle(a, b, c));
    assert(!f.in_triangle_strict(a, b, c));
    assert(!g.in_triangle(a, b, c));
    assert(!g.in_triangle_strict(a, b, c));
    assert(!h.in_triangle(a, b, c));
    assert(!h.in_triangle_strict(a, b, c));

    assert(a.turning_left(b, c) == 1);
    assert(b.turning_left(a, c) == -1);

    const line_t<> l{point_t<>{1, 1}, point_t<>{11, 3}};
    const point_t<> on_line{6, 2};
    assert(on_line.on_segment(l));
    const point_t<> on_line_contin{21, 5};
    assert(!on_line_contin.on_segment(l));
    const point_t<> not_on_line{8, 2};
    assert(!not_on_line.on_segment(l));
    assert(line_t<>::intersects(l, {{1, 3}, {6, 0}}));

    const point_t<double> foot{3, 2}, tf{5, -1};
    const line_t<double> l2{point_t<double>{0, 0}, point_t<double>{6, 4}};
    assert(tf.foot(l2) == foot);

    const point_t<double> u{76, 50};
    const auto u2 = u.unit();
    assert(std::abs(u2.len() - 1) < EPS && std::abs(u2.cross(u)) < EPS);

    #define M_PI 3.14159265358979323846
    const point_t<double> r{100, 0};
    const auto r2 = r.rotate(M_PI / 4);
    const auto r3 = r.rotate(M_PI / -4);
    const auto r4 = r.rotate(M_PI / 2);
    assert(r2.x == r2.y && r2.x > 0 && r2.y > 0);
    assert(r3.x == r2.x && r3.y == -r2.y && r3.x > 0 && r3.y < 0);
    assert(std::abs(r4.x) < EPS && std::abs(r4.y - r.x) < EPS);

    const point_t<double> t1{-2.65, -1.01};
    const point_t<double> t2{4.17, 2.31};
    const point_t<double> t3{0.15, 5.81};
    assert(std::abs(point_t<double>::area(t1, t2, t3) - (t1 - t2).len() * (t3 - t3.foot(line_t<double>{t1, t2})).len() / 2) < EPS);

    std::vector<point_t<>> v = {{0, 0}, {2, 0}, {1, 1}, {1, 2}, {0, 2}};
    assert(polygon_area2(v) == 5);
    assert((convex_hull(v) == std::vector{v[0], v[1], v[3], v[4]}));

    const auto isn = line_t<double>::find_intersection({{0, 0}, {17.3, -2.5}}, {{2, 5.7}, {15.9, -3.14}});
    assert(isn.has_value() && std::abs(isn.value().x - 14.186111252) < EPS && std::abs(isn.value().y + 2.050016076) < EPS);
    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 point.cpp -o point.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address point.cpp -o point

*/

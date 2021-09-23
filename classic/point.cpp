
#include <assert.h>
#include <cmath>
#include <iostream>

// see: /kickstart/2021/star_trappers.cpp

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

int main(int, char**)
{
    point_t<> a{0, 0};
    point_t<> b{4, 0};
    point_t<> c{0, 4};
    point_t<> d{2, 2};
    point_t<> e{1, 1};
    point_t<> f{3, 3};
    point_t<> g{2, -1};
    point_t<> h{-1, 3};

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
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 point.cpp -o point.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address point.cpp -o point

*/

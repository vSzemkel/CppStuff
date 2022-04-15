/*
ID: marcin.8
LANG: C++17
TASK: fc
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=QRAusS0iKMr&S=fc
*/

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <vector>

std::ifstream task_in("fc.in");
std::ofstream task_out("fc.out");

template <typename T = int64_t> struct point_t;
template <typename T = int64_t> using line_t = std::array<point_t<T>, 2>;

template <typename T>
struct point_t {
    point_t(T _x = T{}, T _y = T{}) : x(_x), y(_y) {}
    T cross(const point_t& p) const { return x * p.y - y * p.x; }
    T cross(const point_t& a, const point_t& b) const { return (a - *this).cross(b - *this); }
    T dot(const point_t& p) const { return x * p.x + y * p.y; }
    T dot(const point_t& a, const point_t& b) const { return (a - *this).dot(b - *this); }
    T len() const { return std::sqrt(dot(*this)); }
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

    friend std::ostream& operator<<(std::ostream& out, const point_t& p) { return out << p.x << ' ' << p.y; }
    friend std::istream& operator>>(std::istream& in, point_t& p) { in >> p.x >> p.y; return in; }

    T x, y;
};

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
    int N;
    task_in >> N;
    std::vector<point_t<double>> grazings(N);
    for (auto& p : grazings)
        task_in >> p;

    const auto ch = convex_hull<double>(grazings);
    task_out << std::fixed << std::setprecision(2) << perimeter_len(ch) << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 fc.cpp -o fc.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address fc.cpp -o fc

Input:


Output:


*/

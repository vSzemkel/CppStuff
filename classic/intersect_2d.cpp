
#include <iostream>
#include <format>
#include <optional>
#include <string>

// find intersection between two sections in general position (distinct points, non parallel sections)
// adapted from: https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect

template <typename T = float>
struct point_t { T x, y; };

template <typename T = float>
using section_t = std::pair<point_t<T>, point_t<T>>;

template <typename T = float>
bool intersection_status(const section_t<T>& s1, const section_t<T>& s2)
{
    const T s1_dx = s1.second.x - s1.first.x;
    const T s1_dy = s1.second.y - s1.first.y;
    const T s2_dx = s2.second.x - s2.first.x;
    const T s2_dy = s2.second.y - s2.first.y;

    const T s = (-s1_dy * (s1.first.x - s2.first.x) + s1_dx * (s1.first.y - s2.first.y)) / (-s2_dx * s1_dy + s1_dx * s2_dy);
    if (s < 0 || 1 < s)
        return false;
    const T t = ( s2_dx * (s1.first.y - s2.first.y) - s2_dy * (s1.first.x - s2.first.x)) / (-s2_dx * s1_dy + s1_dx * s2_dy);
    if (t < 0 || 1 < t)
        return false;

    return true;
}

template <typename T = float>
std::optional<point_t<T>> find_intersection(const section_t<T>& s1, const section_t<T>& s2)
{
    const T s1_dx = s1.second.x - s1.first.x;
    const T s1_dy = s1.second.y - s1.first.y;
    const T s2_dx = s2.second.x - s2.first.x;
    const T s2_dy = s2.second.y - s2.first.y;

    const T s = (-s1_dy * (s1.first.x - s2.first.x) + s1_dx * (s1.first.y - s2.first.y)) / (-s2_dx * s1_dy + s1_dx * s2_dy);
    if (s < 0 || 1 < s)
        return std::nullopt;
    const T t = ( s2_dx * (s1.first.y - s2.first.y) - s2_dy * (s1.first.x - s2.first.x)) / (-s2_dx * s1_dy + s1_dx * s2_dy);
    if (t < 0 || 1 < t)
        return std::nullopt;

    return point_t<T>{s1.first.x + (t * s1_dx), s1.first.y + (t * s1_dy)};
}

template <typename T = float>
std::string print_section(const section_t<T>& s)
{
    return std::format("[{:.1f}, {:.1f}]---[{:.1f}, {:.1f}]", s.first.x, s.first.y, s.second.x, s.second.y);
}

template <typename T = float>
void print_intersection(const section_t<T>& s1, const section_t<T>& s2)
{
    std::cout << print_section(s1) << " and " << print_section(s2);
    if (auto inter = find_intersection(s1, s2)) {
        const auto& val = inter.value();
        std::cout << " intersect in [" << val.x << ", " << val.y << "]\n";
    } else
        std::cout << " do not intersect\n";
}

// Lower rectangle occluded by upper gives shape that can be divided to at most 4 rectangles
using rect_t = std::array<int, 4>;
const auto intersection = [&](const rect_t& lower, const rect_t& upper) { // { x, y, X, Y }
    std::vector<rect_t> ret;
    if (upper[2] <= lower[0] || lower[2] <= upper[0] || upper[3] <= lower[1] || lower[3] <= upper[1])
        ret.push_back(lower);
    else {
        if (lower[0] < upper[0])
            ret.push_back({lower[0], lower[1], upper[0], lower[3]});
        if (lower[1] < upper[1])
            ret.push_back({std::max(lower[0], upper[0]), lower[1], std::min(lower[2], upper[2]), upper[1]});
        if (upper[2] < lower[2])
            ret.push_back({upper[2], lower[1], lower[2], lower[3]});
        if (upper[3] < lower[3])
            ret.push_back({std::max(lower[0], upper[0]), upper[3], std::min(lower[2], upper[2]), lower[3]});
    }

    return ret;
};

int main(int argc, char* argv[])
{
    print_intersection<float>({{0, 0}, {10, 0}}, {{5, -5}, {5, 5}});
    print_intersection<float>({{0, 0}, {10, 0}}, {{5, 5}, {10, 0}});
    print_intersection<float>({{0, 0}, {10, 0}}, {{5, 5}, {10, 5}});
    print_intersection<float>({{0, 0}, {10, 0}}, {{5, 5}, {4, 2}});
    print_intersection<float>({{0, 0}, {17.3, -2.5}}, {{2, 5.7}, {15.9, -3.14}});
    return 0;
}

/* clang++.exe -Wall -g -std=c++20 intersect_2d.cpp -o intersect_2d.exe

Output:

[0.0, 0.0]---[10.0, 0.0] and [5.0, -5.0]---[5.0, 5.0] intersect in [5, 0]
[0.0, 0.0]---[10.0, 0.0] and [5.0, 5.0]---[10.0, 0.0] intersect in [10, 0]
[0.0, 0.0]---[10.0, 0.0] and [5.0, 5.0]---[10.0, 5.0] do not intersect
[0.0, 0.0]---[10.0, 0.0] and [5.0, 5.0]---[4.0, 2.0] do not intersect
[0.0, 0.0]---[17.3, -2.5] and [2.0, 5.7]---[15.9, -3.1] intersect in [14.1861, -2.05002]

*/
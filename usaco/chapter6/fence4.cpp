/*
ID: marcin.8
LANG: C++
TASK: fence4
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=szTEXMwIDAp&S=fence4
*/

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <optional>
//#include <numbers>
#include <unordered_map>
#include <utility>
#include <vector>

std::ifstream task_in("fence4.in");
std::ofstream task_out("fence4.out");

/**
 * Observation1: After translating coordinates center to observer location, computations are easier.
 * Observation2: We can scan a scene ccw and analyze encountered points as events
 */

static const auto EPS = 0.000000001;
static const auto LONG_RADIUS = 10'000;
static const auto PI = 3.14159265358979323846;

#pragma region Point and Section
template <typename T = float>
struct point_t {
    T dot() const { return x * x + y * y; }
    T x, y;
};

template <typename T = float>
std::istream& operator>>(std::istream& input, point_t<T>& point) {
    input >> point.x >> point.y;
    return input;
}

using point = point_t<double>;
point observer;

template <typename T = float>
using section_t = std::pair<point_t<T>, point_t<T>>;

template <typename T = float>
std::ostream& operator<<(std::ostream& output, const section_t<T>& section) {
    output << section.first.x + observer.x << ' ' << section.first.y + observer.y<< ' ' << section.second.x + observer.x << ' ' << section.second.y + observer.y;
    return output;
}
#pragma endregion

int N;
std::vector<bool> ans;
std::vector<section_t<double>> fences;

template <typename T = float>
std::optional<point_t<T>> find_intersection(const section_t<T>& s1, const section_t<T>& s2)
{
    const T s1_dx = s1.second.x - s1.first.x;
    const T s1_dy = s1.second.y - s1.first.y;
    const T s2_dx = s2.second.x - s2.first.x;
    const T s2_dy = s2.second.y - s2.first.y;

    const T s = (-s1_dy * (s1.first.x - s2.first.x) + s1_dx * (s1.first.y - s2.first.y)) / (-s2_dx * s1_dy + s1_dx * s2_dy);
    if (s <= 0 || 1 <= s)
        return std::nullopt;
    const T t = ( s2_dx * (s1.first.y - s2.first.y) - s2_dy * (s1.first.x - s2.first.x)) / (-s2_dx * s1_dy + s1_dx * s2_dy);
    if (t <= 0 || 1 <= t)
        return std::nullopt;

    return point_t<T>{s1.first.x + (t * s1_dx), s1.first.y + (t * s1_dy)};
}

double distance_sq(const int fence_id, double angle)
{
    const auto& fence = fences[fence_id];
    angle -= PI;
    point distant{LONG_RADIUS * std::cos(angle), LONG_RADIUS * std::sin(angle)};
    section_t<double> ray{{0, 0}, distant};
    const auto intersection = find_intersection(fence, ray);
    assert(intersection.has_value());
    return intersection->dot();
}

struct event_t {
    int fence_id;
    double angle;
    bool beginning;
    bool operator<(const event_t& other) const { return angle < other.angle; }
};

int main(int, char**)
{
    task_in >> N;
    fences.resize(N);
    ans.resize(N);
    task_in >> observer;
    point first;
    task_in >> first;
    first.x -= observer.x;
    first.y -= observer.y;
    auto prev = first;
    for (int i = 0; i < N - 1; ++i) {
        point next;
        task_in >> next;
        next.x -= observer.x;
        next.y -= observer.y;
        fences[i] = {prev, next};
        prev = next;
    }
    fences[N - 1] = fences[N - 2];
    fences[N - 2] = {first, prev};

    for (int i = 0; i < N; ++i)
        for (int j = i + 2; j < N; ++j)
            if (find_intersection(fences[i], fences[j])) {
                task_out << "NOFENCE\n";
                return 0;
            }

    int f{};
    std::vector<event_t> angles;
    for (const auto& [p0, p1] : fences) {
        auto a0 = PI + std::atan2(p0.y, p0.x);
        auto a1 = PI + std::atan2(p1.y, p1.x);
        if (a0 > a1)
            std::swap(a0, a1);
        if (a1 - a0 < EPS)
            continue;
        if (a1 - a0 >= PI) {
            angles.emplace_back(event_t{f, a1, true});
            angles.emplace_back(event_t{f, 2 * PI, false});
            angles.emplace_back(event_t{f, 0, true});
            angles.emplace_back(event_t{f, a0, false});
        } else {
            angles.emplace_back(event_t{f, a0, true});
            angles.emplace_back(event_t{f, a1, false});
        }

        ++f;
    }

    std::sort(angles.begin(), angles.end());

    std::vector<int> active;
    const int sz = int(angles.size());
    for (int i = 0; i < sz; ++i) {
        const auto& a = angles[i];
        if (a.beginning == false && std::abs(a.angle - 2 * PI) < EPS)
            continue;
        if (a.beginning)
            active.push_back(a.fence_id);
        else
            //std::erase(active, a.fence_id);
            active.erase(std::find(active.begin(), active.end(), a.fence_id));
        if (active.empty())
            continue;

        const int j = i + 1;
        if (j < sz && std::abs(angles[j].angle - a.angle) < EPS)
            continue;
        assert (j < sz);
        const auto angle = (a.angle + angles[j].angle) / 2;

        const auto visible = *std::min_element(active.begin(), active.end(), [&](const int a, const int b) {
             return distance_sq(a, angle) < distance_sq(b, angle);
        });

        ans[visible] = true;
    }

    task_out << std::count(ans.begin(), ans.end(), true) << '\n';
    for (int i = 0; i < N; ++i)
        if (ans[i])
            task_out << fences[i] << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 fence4.cpp -o fence4.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address fence4.cpp -o fence4

Run:
fence4.exe && type fence4.out

Input:

13
5 5
0 0
7 0
5 2
7 5
5 7
3 5
4 9
1 8
2 5
0 9
-2 7
0 3
-3 1

Output:

7
0 0 7 0
5 2 7 5
7 5 5 7
5 7 3 5
-2 7 0 3
0 0 -3 1
0 3 -3 1

*/

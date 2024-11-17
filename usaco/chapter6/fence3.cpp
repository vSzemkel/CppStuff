/*
ID: marcin.8
LANG: C++
TASK: fence3
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=4VbapYMHyQ1&S=fence3
*/

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <vector>

std::ifstream task_in("fence3.in");
std::ofstream task_out("fence3.out");

class fence_t
{
  public:
    static constexpr float SCALE = 10;

    static float distance(const std::vector<fence_t>& fences, const int x, const int y) {
        float d{};
        for (const auto& f : fences)
            d += std::sqrt(f.dist2(x, y));
        return d;
    }

    static std::array<int, 4> bbox(const std::vector<fence_t>& fences) {
        int x0(INF), y0(INF), x1(-INF), y1(-INF);
        for (const auto& f : fences) {
            if (f._x0 < x0) x0 = f._x0;
            else if (x1 < f._x1) x1 = f._x1;
            if (f._y0 < y0) y0 = f._y0;
            else if (y1 < f._y1) y1 = f._y1;
        }
        return {x0, y0, x1, y1};
    }

    friend std::istream& operator>>(std::istream& input, fence_t& p) {
        input >> p._x0 >> p._y0 >> p._x1 >> p._y1;
        p._x0 *= SCALE; p._y0 *= SCALE; p._x1  *= SCALE; p._y1 *= SCALE;
        assert(p._x0 == p._x1 || p._y0 == p._y1);
        if (p._x0 > p._x1) std::swap(p._x0, p._x1);
        if (p._y0 > p._y1) std::swap(p._y0, p._y1);
        return input;
    }

  private:
    float dist2(const int x, const int y) const {
        if (_y0 == _y1) {
            if (x < _x0)
                return (x - _x0) * (x - _x0) + (y - _y0) * (y - _y0);
            else if (_x1 < x)
                return (x - _x1) * (x - _x1) + (y - _y0) * (y - _y0);
            else
                return (y - _y0) * (y - _y0);
        } else {
            if (y < _y0)
                return (y - _y0) * (y - _y0) + (x - _x0) * (x - _x0);
            else if (_y1 < y)
                return (y - _y1) * (y - _y1) + (x - _x0) * (x - _x0);
            else
                return (x - _x0) * (x - _x0);
        }
    }

    static constexpr int INF = 1e09;
    int _x0, _y0, _x1, _y1;
};

int N;
std::vector<fence_t> fences;

int main(int, char**)
{
    task_in >> N;
    fences.resize(N);
    for (auto& f : fences)
        task_in >> f;

    const auto bbox = fence_t::bbox(fences);
    int bx, dx = std::max(1, (bbox[2] - bbox[0]) / 10);
    int by, dy = std::max(1, (bbox[3] - bbox[1]) / 10);
    float bd = 1e09;
    for (int x = bbox[0]; x <= bbox[2]; x += dx)
        for (int y = bbox[1]; y <= bbox[3]; y += dy)
            if (const auto can = fence_t::distance(fences, x, y); can < bd) {
                bd = can;
                bx = x;
                by = y;
            }

    for (int x = std::max(bbox[0], bx - dx); x <= std::min(bbox[2], bx + dx); ++x)
        for (int y = std::max(bbox[1], by - dy); y <= std::min(bbox[3], by + dy); ++y)
            if (const auto can = fence_t::distance(fences, x, y); can < bd) {
                bd = can;
                bx = x;
                by = y;
            }
    
    task_out << std::fixed << std::setprecision(1) << bx / fence_t::SCALE << ' ' << by / fence_t::SCALE << ' ' << bd / fence_t::SCALE << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 fence3.cpp -o fence3.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address fence3.cpp -o fence3

Run:
fence3.exe && type fence3.out

Input:

3
0 0 0 1
2 0 2 1
0 3 2 3

Output:

1.0 1.6 3.7

*/

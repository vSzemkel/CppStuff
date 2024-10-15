/*
ID: marcin.8
LANG: C++
TASK: rect1
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=j4kadQZ1Cxc&S=rect1
*/

#include <algorithm>
#include <array>
#include <fstream>
#include <functional>
#include <map>
#include <vector>

std::ifstream task_in("rect1.in");
std::ofstream task_out("rect1.out");

/**
 * As board color is reported at an additional rectangle at the very bottom
 * Lower rectangle ocluded by upper gives shape that can be divided to at most 4 rectangles
 */
int main(int, char**)
{
    using rect_t = std::array<int, 4>;
    int R, C, N;
    task_in >> C >> R >> N;
    std::map<int, int> ans;
    std::vector<int> taken(R);
    std::vector<int> color(N + 1);
    std::vector<rect_t> rect(N + 1);
    color[0] = 1;
    rect[0] = {0, 0, C, R};
    for (int i = 1; i <= N; ++i) {
        auto& s = rect[i];
        task_in >> s[0] >> s[1] >> s[2] >> s[3] >> color[i];
    }

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

    const std::function<int(const rect_t&, int)> visible_area = [&](const rect_t& r, int pos) {
        if (pos == N)
            return (r[2] - r[0]) * (r[3] - r[1]);

        ++pos;
        int ans{};
        for (const auto& s : intersection(r, rect[pos]))
            ans += visible_area(s, pos);

        return ans;
    };

    for (int i = N; ~i; --i) {
        auto& s = rect[i];
        if (s[2] < s[0])
            std::swap(s[2], s[0]);
        if (s[3] < s[1])
            std::swap(s[3], s[1]);

        const auto va = visible_area(s, i);
        if (0 < va)
            ans[color[i]] += va;
    }

    for (const auto& [color, count] : ans)
        task_out << color << ' ' << count << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 rect1.cpp -o rect1.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address rect1.cpp -o rect1

Run:
rect1.exe && type rect1.out

Input:

20 20 3
2 2 18 18 2
0 8 19 19 3
8 0 10 19 4

Output:

1 91
2 84
3 187
4 38

*/

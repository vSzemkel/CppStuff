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

void solution1()
{
    const int WHITE = 1;
    int R, C, N, total{};
    task_in >> C >> R >> N;
    std::map<int, int> ans;
    std::vector<int> taken(R);
    std::vector<std::vector<bool>> board(R);

    std::vector<std::array<int, 5>> rect(N); // llx, lly, urx, ury, color
    for (auto& r : rect)
        task_in >> r[0] >> r[1] >> r[2] >> r[3] >> r[4];

    for (int i = N - 1; ~i; --i) {
        auto& s = rect[i];
        if (s[2] < s[0])
            std::swap(s[2], s[0]);
        if (s[3] < s[1])
            std::swap(s[3], s[1]);

        for (int r = s[1]; r < s[3]; ++r) {
            auto& row = board[r];
            if (row.empty())
                row.resize(C);
            std::fill_n(row.begin() + s[0], s[2] - s[0], true);
            const auto ntaken = std::count_if(row.begin(), row.end(), [](const bool value){ return value; });
            const auto d = ntaken - taken[r];
            if (s[4] != WHITE && 0 < d) {
                total += d;
                ans[s[4]] += d;
            }
            taken[r] = ntaken;
        }
    }

    task_out << "1 " << R * C - total << '\n';
    for (const auto& [color, count] : ans)
        task_out << color << ' ' << count << '\n';
}

void solution2()
{
    using rect_t = std::array<int, 4>;
    int R, C, N, cnt{};
    task_in >> C >> R >> N;
    std::map<int, int> ans;
    std::vector<int> taken(R);
    std::vector<int> color(N);
    std::vector<rect_t> rect(N); // llx, lly, urx, ury, color
    for (auto& s : rect)
        task_in >> s[0] >> s[1] >> s[2] >> s[3] >> color[cnt++];

    color.push_back(1);
    rect.push_back(rect_t{0, 0, C, R});
    std::rotate(color.begin(), color.end() - 1, color.end());
    std::rotate(rect.begin(), rect.end() - 1, rect.end());

    const auto intersection = [&](const rect_t& lower, const rect_t& upper) {
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

int main(int, char**)
{
    solution2();
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

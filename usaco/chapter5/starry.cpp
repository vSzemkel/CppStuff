/*
ID: marcin.8
LANG: C++
TASK: starry
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=fIKLkNzfFfh&S=starry
*/

#include <cassert>
#include <fstream>
#include <string>
#include <vector>

std::ifstream task_in("starry.in");
std::ofstream task_out("starry.out");

struct cluster_t {
    char symbol;
    short parent;
    short right, bottom, left, top;
    short width, height;
    std::vector<std::pair<short, short>> coord, shape;

    auto to_string() const {
        assert(!left && !top);
        std::string ret(width * height, '0');
        for (const auto& p : shape)
            ret[p.first * width + p.second] = '1';
        return ret;
    }

    auto vertical_swap() const {
        cluster_t ret = *this;
        for (auto& p : ret.shape)
            p.second = right - p.second;
        return ret;
    }

    auto horizontal_swap() const {
        cluster_t ret = *this;
        for (auto& p : ret.shape)
            p.first = bottom - p.first;
        return ret;
    }

    auto rotate() const {
        cluster_t ret = *this;
        for (auto& p : ret.shape)
            std::swap(p.first, p.second);
        std::swap(ret.right, ret.bottom);
        std::swap(ret.width, ret.height);
        return ret;
    }

    void normalize() {
        right -= left;
        bottom -= top;
        shape = coord;
        for (auto& p : shape) {
            p.first -= top;
            p.second -= left;
        }
        left = top = 0;
        width = right + 1;
        height = bottom + 1;
    }
};

int R, C;
std::vector<std::string> sky;
std::vector<cluster_t> clusters;
std::vector<std::vector<bool>> seen;

constexpr const int d8c[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
constexpr const int d8r[8] = {0, 1, 1, 1, 0, -1, -1, -1};

static void flood_fill(const short r, const short c) {
    if (r < 0 || r == R || c < 0 || c == C || seen[r][c] || sky[r][c] == '0')
        return;
    seen[r][c] = true;
    cluster_t& cur = clusters.back();
    cur.left = std::min(cur.left, c);
    cur.right = std::max(cur.right, c);
    cur.top = std::min(cur.top, r);
    cur.bottom = std::max(cur.bottom, r);
    cur.coord.emplace_back(r, c);
    for (int d = 0; d < 8; ++d)
        flood_fill(r + d8r[d], c + d8c[d]);
}

bool are_similar(const int pat, const int pro) {
    const cluster_t& patern = clusters[pat];
    cluster_t probe = clusters[pro];
    if (patern.width * patern.height != probe.width * probe.height 
    || (patern.width != probe.width && patern.width != probe.height)
    ||  patern.shape.size() != probe.shape.size())
        return false;

    int ub{4};
    if (patern.width != probe.width)
        probe = probe.rotate();
    else
        ub <<= 1;

    const auto patstr = patern.to_string();
    for (int i = 0; i < ub; ++i) {
        auto t = probe;
        if (i & 1) t = t.horizontal_swap();
        if (i & 2) t = t.vertical_swap();
        if (i & 4) t = t.rotate();
        if (patstr == t.to_string())
            return true;
    }

    return false;
}

int main(int, char**)
{
    std::string comet, group;
    task_in >> C >> R;
    sky.resize(R);
    for (auto& r : sky)
        task_in >> r;

    // Flood fill to identify clusters
    seen.assign(R, std::vector<bool>(C));
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c)
            if (!seen[r][c] && sky[r][c] == '1' ) {
                cluster_t cur;
                cur.parent = clusters.size();
                cur.left = cur.right = c;
                cur.top = cur.bottom = r;
                clusters.push_back(cur);
                flood_fill(r, c);
            }

    // Normalize clusters
    for (auto& cc : clusters)
        cc.normalize();

    // Recognize similar
    const int sz = int(clusters.size());
    for (int j = 1; j < sz; ++j)
        for (int i = j - 1; ~i; --i)
            if (are_similar(i, j))
                clusters[j].parent = clusters[i].parent;

    // Prepare output
    char sym{'a'};
    auto ans = sky;
    for (int i = 0; i < sz; ++i) {
        auto& cur = clusters[i];
        cur.symbol = (cur.parent == i) ? sym++ : clusters[cur.parent].symbol;
        for (const auto& p : cur.coord)
            ans[p.first][p.second] = cur.symbol;
    }

    for (const auto& a : ans)
        task_out << a << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 starry.cpp -o starry.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address starry.cpp -o starry

Input:

23
15
10001000000000010000000
01111100011111000101101
01000000010001000111111
00000000010101000101111
00000111010001000000000
00001001011111000000000
10000001000000000000000
00101000000111110010000
00001000000100010011111
00000001110101010100010
00000100110100010000000
00010001110111110000000
00100001110000000100000
00001000100001000100101
00000001110001000111000

Output:

a000a0000000000b0000000
0aaaaa000ccccc000d0dd0d
0a0000000c000c000dddddd
000000000c0b0c000d0dddd
00000eee0c000c000000000
0000e00e0ccccc000000000
b000000e000000000000000
00b0f000000ccccc00a0000
0000f000000c000c00aaaaa
0000000ddd0c0b0c0a000a0
00000b00dd0c000c0000000
000g000ddd0ccccc0000000
00g0000ddd0000000e00000
0000b000d0000f000e00e0b
0000000ddd000f000eee000

*/

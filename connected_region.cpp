#include <algorithm>
#include <stdio.h>
#include <vector>

// find maximum connected region
// two cells are connected when they have common edge

// case definition
constexpr int g_rows = 7;
constexpr int g_cols = 20;
constexpr int g_show = 10;
constexpr char g_pattern[] = 
"aaaa---VVBBBBB**kkkk"
"aaa----VVBBB****kkrr"
"a------VVBBB****rrrr"
"aaa----VV*BB***rrrrr"
"aaa------*********rr"
"aaa----4-**4***rrrrr"
"aaaa---4444444**rrrr";

// globals
std::vector<int> g_adj(4);
std::vector<uint8_t> g_visited(g_rows * g_cols, 0);
std::vector<std::vector<int>> g_regions;

// collect 2 to 4 adjacent cells to pos
auto adjacents(const int pos)
{
    const int row = pos / g_cols;
    const int col = pos % g_cols;

    g_adj.clear();
    if (col > 0) 
        g_adj.push_back(pos - 1);
    if (col < g_cols - 1)
        g_adj.push_back(pos + 1);
    if (row > 0)
        g_adj.push_back(pos - g_cols);
    if (row < g_rows - 1)
        g_adj.push_back(pos + g_cols);

    return g_adj;
}

// classic depth first search
void dfs(int pos)
{
    g_visited[pos] = 1;
    g_regions.back().push_back(pos);
    for (const int i : adjacents(pos))
        if (g_visited[i] == 0 && g_pattern[pos] == g_pattern[i])
            dfs(i);
}

// classic depth first search, no recursion
void dfs_nr(const int pos)
{
    char symbol = g_pattern[pos];
    std::vector<int> cache;
    cache.push_back(pos);
    g_visited[pos] = 1;

    while (!cache.empty()) {
        const int p = cache.back();
        cache.pop_back();
        g_regions.back().push_back(p);
        for (const int i : adjacents(p))
            if (g_visited[i] == 0 && g_pattern[i] == symbol) {
                cache.push_back(i);
                g_visited[i] = 1;
            }
    }
}

int main(int argc, char* argv[])
{
    // check input data
    static_assert(sizeof(g_pattern) > 1);
    static_assert(sizeof(g_pattern) > g_show);
    static_assert(sizeof(g_pattern) - 1 == g_rows * g_cols);

    // collect regions
    for (int i = 0; i < g_rows * g_cols; ++i) 
        if (g_visited[i] == 0) {
            g_regions.push_back({});
            dfs_nr(i);
        }

    // select winner
    auto& max_region = *std::max_element(g_regions.begin(), g_regions.end(), 
        [](const auto v1, const auto v2) {
            return v1.size() < v2.size();
        });

    // present result
    printf("\nRecognized %llu connected regions of sizes:\n", g_regions.size());
    for (const auto& v : g_regions)
        printf("\t'%c': %llu\n", g_pattern[v[0]], v.size());
    printf("\nMaximal region of size %llu is composed of symbol '%c'\n", max_region.size(), g_pattern[max_region[0]]);
    printf("First %i cells of that region: (col, row)\n", g_show);
    int i = 0;
    std::make_heap(max_region.begin(), max_region.end(), std::greater<>{});
    while (i < g_show) {
        const auto pos = max_region.front();
        std::pop_heap(max_region.begin(), max_region.end() - i, std::greater<>{});
        printf("\t[#%02i:cell %02i]  (%i, %i)\n", ++i, pos, pos % g_cols, pos / g_cols);
    }

    return 0;
}
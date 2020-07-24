#include <stdio.h>
#include <algorithm>
#include <vector>

// find maximum connected region
// two cells are connected when they have common edge

// case definition
constexpr int g_rows = 7;
constexpr int g_cols = 12;
constexpr char g_pattern[] = 
"aaaa---VVBBB"
"aaa----VVBBB"
"a------VVBBB"
"aaa----VVBBB"
"aaa-------BB"
"aaa----VVBBB"
"aaaa---VVBBB";

// globals
int g_region_id = 0;
std::vector<int> g_adj(4);
std::vector<uint8_t> g_visited(g_rows * g_cols, 0);
std::vector<std::vector<int>> g_regions;

// collect 2 to 4 adjacent cells to pos
auto adjacents(int pos)
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

int main(int argc, char* argv[])
{
    // check input data
    static_assert(sizeof(g_pattern) > 1);
    static_assert(sizeof(g_pattern) - 1 == g_rows * g_cols);

    // collect regions
    for (int i = 0; i < g_rows * g_cols; ++i) 
        if (g_visited[i] == 0) {
            g_regions.push_back({});
            dfs(i);
            ++g_region_id;
        }

    // select winner
    const auto max_region = std::max_element(g_regions.begin(), g_regions.end(), 
        [](const auto v1, const auto v2) {
            return v1.size() < v2.size();
        });

    // present result
    printf("\nMaximum region of size %i is composed of symbol %c\n", max_region->size(), g_pattern[(*max_region)[0]]);
    int i = 1;
    std::sort(max_region->begin(), max_region->end());
    for (const int pos : *max_region)
        printf("%i: (%i, %i)\n", i++, pos % g_cols, pos / g_cols);

    return 0;
}
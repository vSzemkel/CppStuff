#include <algorithm>
#include <stdio.h>
#include <vector>

// find maximum connected region, mark rectangular ones
// two cells are connected when they have common edge

// case definition
constexpr int g_rows = 7;
constexpr int g_cols = 20;
constexpr int g_show = 10;
constexpr char g_pattern[] = 
"aaaa---VVBBBBB**kkkk"
"aaa----VVBBB****kkkk"
"a------VVBBB****rrrr"
"aaa----VV*BB***rrrrr"
"aaa------*********rr"
"aaa----4-**4***rrrrr"
"aaaa---4444444**rrrr";

// globals
std::vector<int> g_adj(4);
std::vector<uint8_t> g_visited(g_rows * g_cols, 0);
std::vector<std::vector<int>> g_regions;

constexpr auto fn_col = [](const int cell){ return cell % g_cols; };
constexpr auto fn_row = [](const int cell){ return cell / g_cols; };

// collect 2 to 4 adjacent cells to pos
auto adjacents(const int pos)
{
    const int row = fn_row(pos);
    const int col = fn_col(pos);

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
void dfs(const int pos)
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
    std::vector<int> cache{pos};
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

bool is_rectangular(const std::vector<int>& region)
{
    const auto col_span = std::minmax_element(region.begin(), region.end(), [](const int cell1, const int cell2){ return cell1 % g_cols < cell2 % g_cols; });
    const auto row_span = std::minmax_element(region.begin(), region.end(), [](const int cell1, const int cell2){ return cell1 / g_cols < cell2 / g_cols; });
    return (fn_col(*col_span.second - *col_span.first) + 1) * (fn_row(*row_span.second - *row_span.first) + 1) == region.size();
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
    const char* rect_msg = "RECTANGULAR";
    printf("\nRecognized %zu connected regions of sizes:\n", g_regions.size());
    for (const auto& v : g_regions) {
        const char* r = "";
        if (is_rectangular(v))
            r = rect_msg;
        printf("    '%c': %zu %s\n", g_pattern[v[0]], v.size(), r);
    }
    printf("\nMaximal region of size %zu is composed of symbol '%c'\n", max_region.size(), g_pattern[max_region[0]]);
    printf("First %i cells of that region: (col, row)\n", g_show);
    int i = 0;
    std::make_heap(max_region.begin(), max_region.end(), std::greater<>{});
    while (i < g_show) {
        const auto pos = max_region.front();
        std::pop_heap(max_region.begin(), max_region.end() - i, std::greater<>{});
        printf("    [#%02i:cell %02i]  (%i, %i)\n", ++i, pos, fn_col(pos), fn_row(pos));
    }

    return 0;
}

/*    clang++.exe -Wall -g -std=c++17 connected_region.cpp -o connected_region.exe

Output:

Recognized 8 connected regions of sizes:
    'a': 21 
    '-': 31 
    'V': 8 RECTANGULAR
    'B': 13 
    '*': 30 
    'k': 8 RECTANGULAR
    'r': 20 
    '4': 9

Maximal region of size 31 is composed of symbol '-'
First 10 cells of that region: (col, row)
        [#01:cell 04]  (4, 0)
        [#02:cell 05]  (5, 0)
        [#03:cell 06]  (6, 0)
        [#04:cell 23]  (3, 1)
        [#05:cell 24]  (4, 1)
        [#06:cell 25]  (5, 1)
        [#07:cell 26]  (6, 1)
        [#08:cell 41]  (1, 2)
        [#09:cell 42]  (2, 2)
        [#10:cell 43]  (3, 2)
*/
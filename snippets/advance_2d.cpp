int64_t g_rows, g_cols;
const auto col = [](const int64_t pos) noexcept { return pos % g_cols; };
const auto row = [](const int64_t pos) noexcept { return pos / g_cols; };

static int64_t advance(const int64_t pos, const char dir)
{
    switch (dir) {
        case 'E': return (col(pos) + 1 == g_cols) ? -1 : pos + 1;
        case 'W': return (col(pos) == 0) ? -1 : pos - 1;
        case 'S': return (row(pos) + 1 == g_rows) ? -1 : pos + g_cols;
        case 'N': return (row(pos) == 0) ? -1 : pos - g_cols;
    }
    return -1;
}

/**********************************************************************/

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

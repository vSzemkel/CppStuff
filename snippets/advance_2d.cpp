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

int64_t g_rows, g_cols;
std::vector<int> g_board;

    std::unordered_map<char, std::vector<int>> neighbours;
    g_board.resize(size);
    neighbours['N'].resize(size);
    neighbours['E'].resize(size);
    neighbours['S'].resize(size);
    neighbours['W'].resize(size);
    for (int i = 0; i < size; ++i) {
        int taken; std::cin >> taken;
        g_board[i] = taken;
        if (taken)
            neighbours['N'][i] = neighbours['E'][i] = neighbours['S'][i] = neighbours['W'][i] = 1;
    }

    for (int64_t r = 0; r < g_rows; ++r)
        for (int64_t c = 0; c < g_cols; ++c) {
            const int64_t cell = r * g_cols + c;
            if (!g_board[cell]) continue;
            if (c > 0)
                neighbours['W'][cell] = neighbours['W'][cell - 1] + 1;
            if (r > 0)
                neighbours['N'][cell] = neighbours['N'][cell - g_cols] + 1;
        }
    for (int64_t r = g_rows - 1; r >= 0; --r)
        for (int64_t c = g_cols - 1; c >= 0; --c) {
            const int64_t cell = r * g_cols + c;
            if (!g_board[cell]) continue;
            if (c < g_cols - 1)
                neighbours['E'][cell] = neighbours['E'][cell + 1] + 1;
            if (r < g_rows - 1)
                neighbours['S'][cell] = neighbours['S'][cell + g_cols] + 1;
        }

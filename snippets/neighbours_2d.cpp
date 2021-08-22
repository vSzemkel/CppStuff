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


    // board with walls
    const int size = R * C;
    std::unordered_map<char, std::vector<int>> wall = {
        {'N', std::vector<int>(size, 1)},
        {'S', std::vector<int>(size, 1)},
        {'E', std::vector<int>(size, 1)},
        {'W', std::vector<int>(size, 1)}
    };
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c) {
            const auto cell = r * C + c;
            if (board[r][c] == '#') {
                wall['N'][cell] = wall['W'][cell] = wall['S'][cell] = wall['E'][cell] = 0;
                continue;
            }
            if (c > 0)
                wall['W'][cell] = wall['W'][cell - 1] + 1;
            if (r > 0)
                wall['N'][cell] = wall['N'][cell - C] + 1;
        }
    for (int r = R - 1; r >= 0; --r)
        for (int c = C - 1; c >= 0; --c) {
            const auto cell = r * C + c;
            if (board[r][c] == '#')
                continue;
            if (c < C - 1)
                wall['E'][cell] = wall['E'][cell + 1] + 1;
            if (r < R - 1)
                wall['S'][cell] = wall['S'][cell + C] + 1;
        }

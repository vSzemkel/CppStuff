
constexpr const int d4c[4] = {0, 1, 0, -1}, d4r[4] = {-1, 0, 1, 0}; // 0 == N; 1 == E; 2 == S; 3 == W clockwise
constexpr const int d4c[4] = {0, -1, 0, 1}, d4r[4] = {-1, 0, 1, 0}; // 0 == N; 1 == W; 2 == S; 3 == E counter clockwise
constexpr const int d8c[8] = {-1, -1, 0, 1, 1, 1, 0, -1}, d8r[8] = {0, 1, 1, 1, 0, -1, -1, -1};

static void flood_fill(const short r, const short c) {
    if (r < 0 || r == R || c < 0 || c == C || seen[r][c] || sky[r][c] == '0')
        return;
    seen[r][c] = true;
    /* ... */
    for (int d = 0; d < 8; ++d)
        flood_fill(r + d8r[d], c + d8c[d]);
}


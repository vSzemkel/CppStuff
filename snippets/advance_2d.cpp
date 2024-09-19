
// For read the maze see /usaco/chapter2/maze1.cpp

constexpr const int cW = 1;
constexpr const int cN = 2;
constexpr const int cE = 4;
constexpr const int cS = 8;

std::ostream& operator<<(std::ostream& os, const int course) {
    return os << "-WN-E---S"[course];
}


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

/**************************** MAZE ************************************/

// see: /kickstart/2022/hamiltonian_tour.cpp
const char* map = "SENW";
const int dr[4] = {1, 0, -1, 0}; // row, sorted as map
const int dc[4] = {0, 1, 0, -1}; // col, sorted as map
const int dr[8] = {1, 0, -1, 0, 1, 0, -1, 0}; // cyclic, prevent %
const int dc[8] = {0, 1, 0, -1, 0, 1, 0, -1}; // cyclic, prevent %
const int right_wall[4] = {-1, 0, 1, 2};
std::unordered_map<char, int> dir = {{'S', 0}, {'E', 1}, {'N', 2}, {'W', 3}};
std::map<std::pair<int, int>, char> label = {{{1, 0}, 'S'}, {{0, 1}, 'E'}, {{-1, 0}, 'N'}, {{0, -1}, 'W'}, };

/**********************************************************************/

constexpr auto fn_col = [](const int cell){ return cell % g_cols; };
constexpr auto fn_row = [](const int cell){ return cell / g_cols; };
constexpr std::pair<int, int> fn_rc = [](const int cell){
    const div_t rc = div(cell, g_cols);
    return {rc.quot, rc.rem};
}

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

/**********************************************************************/

constexpr const int d4c[4] = {0, 1, 0, -1}, d4r[4] = {-1, 0, 1, 0}; // 0 == N; 1 == E; 2 == S; 3 == W clockwise
constexpr const int d4c[4] = {0, -1, 0, 1}, d4r[4] = {-1, 0, 1, 0}; // 0 == N; 1 == W; 2 == S; 3 == E counter clockwise
constexpr const int d8c[8] = {-1, -1, 0, 1, 1, 1, 0, -1}, d8r[8] = {0, 1, 1, 1, 0, -1, -1, -1};

// collect 2 to 4 adjacent cells to pos - short but too many comparitions
auto adjacents(const int pos)
{
    const int row = fn_row(pos);
    const int col = fn_col(pos);

    g_adj.clear();
    for (int i = 0; i < 4; ++i) {
        const int nr = row + d4r[i] * g_cols;
        const int nc = col + d4c[i];
        if (0 <= nr && 0 <= nc && nr < g_rows && nc < g_cols)
            g_adj.push_back(nr * g_cols + nc);
    }

    return g_adj;
}

/**********************************************************************/

struct state_t {
    int row, col;
    int dir{0}; // 0 == N; 1 == E; 2 == S; 3 == W
    bool operator==(const state_t& other) const { return row == other.row && col == other.col; }
    void advance() {
        const int next_r = row + d4r[dir];
        const int next_c = col + d4c[dir];
        if (next_r < 0 || next_r == size || next_c < 0 || next_c == size || grid[next_r][next_c] == T)
            dir = (dir + 1) % 4;
        else {
            row = next_r;
            col = next_c;
        }
    }

  private:
    static constexpr const int d4c[4] = {0, 1, 0, -1}, d4r[4] = {-1, 0, 1, 0}; // 0 == N; 1 == E; 2 == S; 3 == W clockwise
};

/**********************************************************************/
// see: /kickstart/2022/pizza_delivery.cpp

constexpr int N{0}, E{1}, W{2}, S{3};
constexpr int DR[] = {-1, 0, 0, +1};
constexpr int DC[] = {0, +1, -1, 0};

for (int dir = 0; dir < 4; ++dir) {
    const int nr = r + DR[dir];
    const int nc = c + DC[dir];
    if (~nr && ~nc && nr < L && nc < L) {
        const auto ns = calculate_toll(dp[pos][m], tolls[dir]);
        // ...
    }
}

/************************ NO WAY BACK***********************************/

constexpr const int N{0}, E{1}, S{2}, W{3};
constexpr const int DR[] = {-1, 0, +1, 0};
constexpr const int DC[] = {0, +1, 0, -1};

for (int z = 3, dir = (s.avoid + 1) % 4; z; --z, dir = (dir + 1) % 4) {
    const auto nr = s.row + DR[dir];
    const auto nc = s.column + DC[dir];
    if (0 <= nr && nr < 4 && 0 <= nc && nc < SZ && !visited[vertex]) {
    }
}

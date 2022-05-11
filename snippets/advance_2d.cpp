
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
std::unordered_map<char, int> dir = {{'S', 0}, {'E', 1}, {'N', 2}, {'W', 3}};
const int dr[4] = {1, 0, -1, 0}; // row "SENW"
const int dc[4] = {0, 1, 0, -1}; // col
const int right_wall[4] = {-1, 0, 1, 2};

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

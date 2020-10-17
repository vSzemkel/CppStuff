
#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

// Bacterial Tactics
// https://codingcompetitions.withgoogle.com/codejam/round/00000000000516b9/0000000000134cdf


enum class cell_t : char {
    empty = '.',
    taken = '*',
    poisoned = '#'
};

class game_t {
  public:
    game_t(int r, int c);
    void set(const int cell, const cell_t status);
    int mark_h(const int cell);
    int mark_v(const int cell);
    void shrink();

    uint64_t hash() const;
    int size() const { return rows * cols; };
    cell_t get(const int c) const { return cells[c]; };
    bool is_empty(const int c) const { return cells[c] == cell_t::empty; };

  private:
    int rows;
    int cols;
    std::vector<cell_t> cells;
    bool can_cut_row(const int row) const;
    bool can_cut_col(const int col) const;
    bool is_accessible(const int cell) const;
    int fn_col(const int cell) const { return cell % cols; };
    int fn_row(const int cell) const { return cell / cols; };
};

game_t::game_t(int r, int c) : rows(r), cols(c), cells(r * c)
{
}

uint64_t game_t::hash() const
{
    const auto mapper = [](cell_t status) {
        if (status == cell_t::empty) return 0;
        if (status == cell_t::taken) return 1;
        return 2;
    };

    if (cells.size() > 28)
        return 0;

    uint64_t dim = ((rows & 0x0f) << 4) + (cols & 0x0f);
    for (const auto s : cells)
        dim = (dim << 2) + mapper(s);

    return dim;
}

int game_t::mark_h(const int cell)
{
    int ret{0};
    auto row = fn_row(cell); // no poisoned cells
    for (int c = cell; fn_row(c) == row; ++c) {
        auto& state = cells[c];
        if (state == cell_t::poisoned) return false;
        if (state == cell_t::taken) break;
        state = cell_t::taken;
        ++ret;
    }

    for (int c = cell - 1; c >= 0 && fn_row(c) == row; --c) {
        auto& state = cells[c];
        if (state == cell_t::poisoned) return false;
        if (state == cell_t::taken) break;
        state = cell_t::taken;
        ++ret;
    }

    return ret;
}

int game_t::mark_v(const int cell)
{
    int ret{0};
    for (int c = cell; c < cells.size(); c += cols) {
        auto& state = cells[c];
        if (state == cell_t::poisoned) return false;
        if (state == cell_t::taken) break;
        state = cell_t::taken;
        ++ret;
    }
        
    for (int c = cell - cols; c >= 0; c -= cols) {
        auto& state = cells[c];
        if (state == cell_t::poisoned) return false;
        if (state == cell_t::taken) break;
        state = cell_t::taken;
        ++ret;
    }

    return ret;
}

bool game_t::is_accessible(const int cell) const {
    if (cells[cell] != cell_t::empty) return false;
    const auto row = fn_row(cell);
    const auto col = fn_col(cell);
    int mulr{1};
    if (col > 0) mulr *= (int)cells[cell - 1];
    if (col < cols - 1) mulr *= (int)cells[cell + 1];
    bool row_poisoned = mulr % (int)cell_t::poisoned == 0;
    int mulc{1};
    if (row > 0) mulc *= (int)cells[cell - cols];
    if (row < rows - 1) mulc *= (int)cells[cell + cols];
    bool col_poisoned = mulc % (int)cell_t::poisoned == 0;
    if (!col_poisoned || !row_poisoned) return true;
    return (mulr % (int)cell_t::empty) == 0 || (mulc % (int)cell_t::empty) == 0;
}

bool game_t::can_cut_row(const int row) const {
    const auto begin = row * cols;
    const auto end = begin + cols;
    for (int c = begin; c < end; ++c) {
        const auto state = cells[c];
        if (is_accessible(c))
            return false;
        if (rows <= 1)
            continue;

        if (state == cell_t::poisoned
             && ((row > 0 && cells[c - cols] == cell_t::empty)
             || (row < rows - 1 && cells[c + cols] == cell_t::empty)))
            return false;
        else if (state == cell_t::taken && row > 0 && row < rows - 1) {
            const auto mul = (int)cells[c - cols] * (int)cells[c + cols];
            if (mul == (int)cell_t::empty * (int)cell_t::poisoned
             || mul == (int)cell_t::empty * (int)cell_t::empty)
            return false;
        }
    }

    return true;
}

bool game_t::can_cut_col(const int col) const {
    const auto end = rows * cols;
    for (int c = col; c < end; c += cols) {
        const auto state = cells[c];
        if (is_accessible(c))
            return false;
        if (cols <= 1)
            continue;

        if (state == cell_t::poisoned
             && ((col > 0 && cells[c - 1] == cell_t::empty)
             || (col < cols - 1 && cells[c + 1] == cell_t::empty)))
            return false;
        else if (state == cell_t::taken && col > 0 && col < cols - 1) {
            const auto mul = (int)cells[c - 1] * (int)cells[c + 1];
            if (mul == (int)cell_t::empty * (int)cell_t::poisoned
             || mul == (int)cell_t::empty * (int)cell_t::empty)
            return false;
        }
    }

    return true;
}

void game_t::shrink()
{
    for (int c = cols - 1; c >= 0; --c)
        if (can_cut_col(c)) {
            for (int r = rows - 1; r >= 0; --r)
                cells.erase(cells.begin() + r * cols + c, cells.begin() + r * cols + c + 1);
            --cols;
        }

    for (int r = rows - 1; r >= 0; --r)
        if (can_cut_row(r)) {
            cells.erase(cells.begin() + r * cols, cells.begin() + (r + 1) * cols);
            --rows;
        }
}

void game_t::set(const int cell, const cell_t status)
{
    cells[cell] = status;
}

std::map<uint64_t, uint8_t> g_game_cache;

int play(const game_t& board, const bool initial)
{
    auto hash = board.hash();
    auto cached = g_game_cache.find(hash);
    if (cached != g_game_cache.end()) 
        return cached->second;

    int won{0};
    auto checked_h = board;
    auto checked_v = board;
    for (int c = 0; c < board.size(); ++c) {
        if (!board.is_empty(c))
            continue;

        // try H colony
        if (checked_h.is_empty(c)) {
            const auto marked = checked_h.mark_h(c);
            if (marked > 0) {
                auto copy = board;
                copy.mark_h(c);
                copy.shrink();
                if (!play(copy, false)) {
                    if (hash != 0)
                        g_game_cache[hash] = marked;
                    if (!initial) return 1;
                    won += marked;
                }
            }
        }
        // try V colony
        if (checked_v.is_empty(c)) {
            const auto marked = checked_v.mark_v(c);
            if (marked > 0) {
                auto copy = board;
                copy.mark_v(c);
                copy.shrink();
                if (!play(copy, false)) {
                    if (hash != 0)
                        g_game_cache[hash] = marked;
                    if (!initial) return 1;
                    won += marked;
                }
            }
        }
    }

    if (hash != 0)
        g_game_cache[hash] = (won > 0) ? 1 : 0;

    return won;
}

int main(int argc, char* argv[])
{
    // parse console input
    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        // read next game board, remember poisoned rows and cols
        cell_t cell;
        int rows, cols, cell_no{0};
        std::cin >> rows >> cols;
        game_t game{rows, cols};
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c) {
                std::cin >> (char&)cell;
                game.set(cell_no++, cell);
            }

        // solve
        g_game_cache.clear();
        std::cout << "Case #" << g << ": " << play(game, true) << "\n";
    }
}

/* clang++.exe -Wall -g -O0 -std=c++17 bacterial_tactics.cpp -o bacterial_tactics.exe
bacterial_tactics.exe < bacterial_tactics.in

Input:

8
6 6
##....
#.....
...#..
#.....
......
...#..
5 5
##...
#....
...#.
#....
.....
5 3
...
###
...
###
...
2 2
..
.#
4 4
.#..
..#.
#...
...#
3 4
#.##
....
#.##
1 1
.
1 2
##


Output:

Case #1: 18
Case #2: 5
Case #3: 9
Case #4: 0
Case #5: 0
Case #6: 7
Case #7: 2
Case #8: 0

*/
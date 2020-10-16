
#include <algorithm>
#include <iostream>
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
    void shrink_h();
    void shrink_v();

    int size() const { return rows * cols; };
    cell_t get(const int c) const { return cells[c]; };
    bool is_empty(const int c) const { return cells[c] == cell_t::empty; };

  private:
    int rows;
    int cols;
    std::vector<cell_t> cells;
    bool can_cut_off(const int row, const int col) const;
    int fn_col(const int cell) const { return cell % cols; };
    int fn_row(const int cell) const { return cell / cols; };
};

game_t::game_t(int r, int c) : rows(r), cols(c), cells(r * c)
{
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

bool game_t::can_cut_off(const int row, const int col) const
{
    const auto cell = row * cols + col;
    const auto state = cells[cell];
    if (state == cell_t::empty) return false;

    return (col == 0 || cells[cell - 1] != cell_t::empty)
        && (col == cols - 1 || cells[cell + 1] != cell_t::empty)
        && (row == 0 || cells[cell - cols] != cell_t::empty)
        && (row == rows - 1 || cells[cell + cols] != cell_t::empty);
}

void game_t::shrink_v()
{
    int c;
    std::vector<int> cut_offs;
    for (int r = 0; r < rows; ++r) {
        for (c = 0; c < cols; ++c)
            if (!can_cut_off(r, c))
                break;

        if (c == cols) 
            cut_offs.push_back(r);
    }

    if (!cut_offs.empty()) {
        for (auto it = cut_offs.rbegin(); it != cut_offs.rend(); ++it)
            cells.erase(cells.begin() + *it * cols, cells.begin() + (*it + 1) * cols);
        rows -= cut_offs.size();
    }
}

void game_t::shrink_h()
{
    int r;
    std::vector<int> cut_offs;
    for (int c = 0; c < cols; ++c) {
       for (r = 0; r < rows; ++r)
            if (!can_cut_off(r, c))
                break;

        if (r == rows)
            cut_offs.push_back(c);
    }

    if (!cut_offs.empty()) {
        for (auto it = cut_offs.rbegin(); it != cut_offs.rend(); ++it) {
            for (r = rows - 1; r >= 0; --r)
                cells.erase(cells.begin() + r * cols + *it, cells.begin() + r * cols + *it + 1);
            --cols;
        }
    }
}

void game_t::set(const int cell, const cell_t status)
{
    cells[cell] = status;
}

int play(const game_t& board, const bool initial)
{
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
                copy.shrink_h();
                copy.shrink_v();
                if (!play(copy, false)) {
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
                copy.shrink_h();
                copy.shrink_v();
                if (!play(copy, false)) {
                    if (!initial) return 1;
                    won += marked;
                }
            }
        }
    }

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
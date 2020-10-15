
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
    void set(const int cell_no, const cell_t status);
    int mark_h(const int cell);
    int mark_v(const int cell);
    void shrink_h();
    void shrink_v();

    int size() const { return rows * cols; };
    cell_t get(const int c) const { return cells[c]; };
    bool is_empty(const int c) const { return cells[c] == cell_t::empty; };
    bool row_safe(const int c) const { return poison_rows[fn_row(c)] == cell_t::empty; };
    bool col_safe(const int c) const { return poison_cols[fn_col(c)] == cell_t::empty; };

  private:
    int rows;
    int cols;
    std::vector<cell_t> cells;
    std::vector<cell_t> poison_rows;
    std::vector<cell_t> poison_cols;
    int fn_col(const int cell) const { return cell % cols; };
    int fn_row(const int cell) const { return cell / cols; };
};

game_t::game_t(int r, int c) : rows(r), cols(c), cells(r * c),
    poison_rows(r, cell_t::empty), poison_cols(c, cell_t::empty)
{
}

int game_t::mark_h(const int cell)
{
    int ret{0};
    auto row = fn_row(cell); // no poisoned cells
    for (int c = cell; fn_row(c) == row; ++c) {
        if (cells[c] == cell_t::taken) break;
        cells[c] = cell_t::taken;
        ++ret;
    }

    for (int c = cell - 1; c >= 0 && fn_row(c) == row; --c) {
        if (cells[c] == cell_t::taken) break;
        cells[c] = cell_t::taken;
        ++ret;
    }

    return ret;
}

int game_t::mark_v(const int cell)
{
    int ret{0};
    for (int c = cell; c < cells.size(); c += cols) {
        if (cells[c] == cell_t::taken) break;
        cells[c] = cell_t::taken;
        ++ret;
    }
        
    for (int c = cell - cols; c >= 0; c -= cols) {
        if (cells[c] == cell_t::taken) break;
        cells[c] = cell_t::taken;
        ++ret;
    }

    return ret;
}

void game_t::shrink_v()
{
    while (rows > 0 && std::all_of(cells.begin(), cells.begin() + cols, [](const auto status){ return status == cell_t::taken; })) {
        cells = std::vector<cell_t>(cells.begin() + cols, cells.end());
        poison_rows = std::vector<cell_t>(poison_rows.begin() + 1, poison_rows.end());
        --rows;
    }

    while (rows > 0 && std::all_of(cells.begin() + (rows - 1) * cols, cells.end(), [](const auto status){ return status == cell_t::taken; })) {
        cells = std::vector<cell_t>(cells.begin(), cells.end() - cols);
        poison_rows = std::vector<cell_t>(poison_rows.begin(), poison_rows.end() - 1);
        --rows;
    }
}

void game_t::shrink_h()
{
    bool effective{false};

    while (cols > 0) { // first column
        for (int r = 0; r < rows; ++r)
            if (cells[r * cols] != cell_t::taken) 
                goto try_last;

        std::vector<cell_t> tmp;
        for (int r = 0; r < rows; ++r)
            tmp.insert(tmp.end(), cells.begin() + r * cols + 1, cells.begin() + (r + 1) * cols);
        cells = tmp;
        poison_cols = std::vector<cell_t>(poison_cols.begin() + 1, poison_cols.end());
        effective = true;
        --cols;
    }

try_last:
    while (cols > 0) { // last column
        for (int r = 0; r < rows; ++r)
            if (cells[cols * (r + 1) - 1] != cell_t::taken) 
                return;

        std::vector<cell_t> tmp;
        for (int r = 0; r < rows; ++r)
            tmp.insert(tmp.end(), cells.begin() + r * cols, cells.begin() + cols * (r  + 1) - 1);
        cells = tmp;
        poison_cols = std::vector<cell_t>(poison_cols.begin(), poison_cols.end() - 1);
        effective = true;
        --cols;
    }

    if (effective)
        shrink_h();
}

void game_t::set(const int cell_no, const cell_t status)
{
    if (status == cell_t::poisoned) {
        poison_rows[fn_row(cell_no)] = status;
        poison_cols[fn_col(cell_no)] = status;
    }
    cells[cell_no] = status;
}

int play(const game_t& board, bool initial)
{
    int won{0};
    auto checked_h = board;
    auto checked_v = board;
    for (int c = 0; c < board.size(); ++c) {
        if (!board.is_empty(c))
            continue;

        // try H colony
        if (board.row_safe(c) && checked_h.is_empty(c)) {
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
        if (board.col_safe(c) && checked_v.is_empty(c)) {
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
    /*game_t test{1, 3};
    test.set(0, cell_t::empty);
    test.set(1, cell_t::taken);
    test.set(2, cell_t::taken);
    test.shrink_h();*/

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
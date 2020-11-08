
#include <algorithm>
#include <array>
#include <assert.h>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <fstream>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <random>
#include <set>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// Painter's Duel
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ff48/00000000003f47fb

constexpr int g_max_size = 10;

enum class cell_t : int8_t {
    bob = -1,
    free = 0,
    alice = 1,
    locked = std::numeric_limits<int8_t>::max()
};

struct game_state_t {
    std::vector<cell_t> board;
    uint8_t last_a, last_b;
    cell_t next_player = cell_t::alice;
};

class game_t {
  public:
    game_t(uint8_t size = 0);
    void mark(uint8_t row, uint8_t pos, cell_t mark);
    const game_state_t& get_state() const { return _state; };
    std::vector<uint8_t> adjacent(uint8_t ind) const;
    static int8_t result(const game_state_t& state);
    static cell_t oponent(cell_t player);
    static int8_t worst_result(cell_t player);
    bool _finishing;
  private:
    uint8_t coord_to_ind(uint8_t row, uint8_t pos) const;

    uint8_t _size;
    game_state_t _state;
    std::array<uint8_t, g_max_size + 1> _triangle_size;
};

game_t::game_t(const uint8_t size) : _finishing(false), _size(size)
{
    assert(size <= g_max_size);
    _triangle_size[0] = 0;
    for (auto i = 1; i <= size; ++i)
        _triangle_size[i] = _triangle_size[i - 1] + 2 * i - 1;

    _state.board.assign(_triangle_size[size], cell_t::free);
}

void game_t::mark(const uint8_t row, const uint8_t pos, const cell_t mark)
{
    const auto ind = coord_to_ind(row, pos);
    _state.board[ind] = mark;
    if (mark == cell_t::alice)
        _state.last_a = ind;
    if (mark == cell_t::bob)
        _state.last_b = ind;
}

uint8_t game_t::coord_to_ind(const uint8_t row, const uint8_t pos) const
{
    uint8_t ret{0};
    if (row > 1)
        ret += _triangle_size[row - 1];

    return ret + pos - 1;
}

std::vector<uint8_t> game_t::adjacent(const uint8_t ind) const
{
    uint8_t row{1};
    while (_triangle_size[row] <= ind)
        ++row;
    uint8_t pos = ind - _triangle_size[row - 1] + 1;

    std::vector<uint8_t> ret;
    if (pos > 1) ret.push_back(ind - 1);
    if (pos < 2 * row - 1) ret.push_back(ind + 1);
    if (row > 1 && pos % 2 == 0) ret.push_back(_triangle_size[row - 2] + pos - 2);
    if (row < _size && pos % 2 == 1) ret.push_back(_triangle_size[row] + pos);
    return ret;
}

int8_t game_t::result(const game_state_t& state)
{
    const auto& b = state.board;
    return std::accumulate(b.begin(), b.end(), 0, [](const auto acu, const auto val) {
        const auto v = (int)val;
        return v <= 1 ? acu + v : acu; }
    );
}

cell_t game_t::oponent(cell_t player)
{
    assert(player == cell_t::alice || player == cell_t::bob);
    return static_cast<cell_t>(-1 * (int8_t)player);
}

int8_t game_t::worst_result(cell_t player) {
    return player == cell_t::alice ? std::numeric_limits<int8_t>::min() : std::numeric_limits<int8_t>::max();
}

game_t g_game;

int solve(const game_state_t& state) {
    int ret = game_t::worst_result(state.next_player);
    const int last_position = state.next_player == cell_t::alice ? state.last_a : state.last_b;

    for (const auto pos : g_game.adjacent(last_position)) 
        if (state.board[pos] == cell_t::free) {
            g_game._finishing = false;
            game_state_t next_move{state};
            next_move.board[pos] = state.next_player;
            if (state.next_player == cell_t::alice) {
                next_move.last_a = pos;
                next_move.next_player = cell_t::bob;
            } else {
                next_move.last_b = pos;
                next_move.next_player = cell_t::alice;
            }

            auto next_result = solve(next_move);
            if (next_result != game_t::worst_result(next_move.next_player))
                ret = state.next_player == cell_t::alice
                    ? std::max(ret, next_result)
                    : std::min(ret, next_result);
        };

    if (ret == game_t::worst_result(state.next_player) && !g_game._finishing) { // try repeat
        game_state_t next_move{state};
        next_move.next_player = game_t::oponent(next_move.next_player);
        g_game._finishing = true;
        ret = solve(next_move);
        if (ret == game_t::worst_result(next_move.next_player))
            return game_t::result(next_move);
    }

    return ret;
}

int main(int argc, char* argv[])
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        int s, ra, pa, rb, pb, c;
        std::cin >> s >> ra >> pa >> rb >> pb >> c;
        g_game = game_t{(uint8_t)s};
        g_game.mark(ra, pa, cell_t::alice);
        g_game.mark(rb, pb, cell_t::bob);
        for (int i = 0; i < c; ++i) {
            std::cin >> ra >> pa;
            g_game.mark(ra, pa, cell_t::locked);
        }
        // Set 1
        std::cout << "Case #" << g << ": " << solve(g_game.get_state()) << "\n";
    }
}

/*
clang++.exe -Wall -ggdb3 -O0 -std=c++17 painters_duel.cpp -o painters_duel.exe
painters_duel.exe < painters_duel.in

Input:

7
2 1 1 2 1 0
2 2 2 1 1 2
2 1
2 3
3 3 4 2 1 2
2 3
3 1
3 3 2 2 3 2
2 1
3 1
2 1 1 2 2 0
2 2 2 1 1 0
2 2 1 2 3 1
1 1

Output:

Case #1: 2
Case #2: 0
Case #3: 0
Case #4: -1
Case #5: -1
Case #6: 1
Case #7: 1

*/

/*
ID: marcin.8
LANG: C++17
TASK: frameup
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=GVqXl5lUbGQ&S=frameup
*/

#include <algorithm>
#include <cassert>
#include <fstream>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

std::ifstream task_in("frameup.in");
std::ofstream task_out("frameup.out");

int R, C;
std::string ans;
std::vector<std::string> board;
std::map<char, unsigned int> covers; // sorted {char, charbits covered}
std::unordered_map<char, std::pair<int, int>> topleft; // {char, top-left rectangle corner}
std::unordered_map<char, int> right, top, left, bottom; // {char, known extent position}
std::vector<std::vector<unsigned int>> N, E, S, W; // neibors on direction

inline static unsigned int charbit(const char c) {
    return 1 << (c - 'A');
}

inline static bool has_hpeer(const unsigned int m, const int r, const int c) {
    return (W[r][c] & m) || (E[r][c] & m);
}

inline static bool has_vpeer(const unsigned int m, const int r, const int c) {
    return (S[r][c] & m) || (N[r][c] & m);
}

static bool identify_rectangle(const char c) {
    const auto r = right[c], t = top[c], l = left[c], b = bottom[c];
    const auto h = b - t, w = r - l;
    if (h < 2 || w < 2)
        return false;

    const unsigned int m = charbit(c);
    int d, s, badr{}, badc{}, score{4};
    if (t > 0) {
        for (d = l; d <= r; ++d)
            if (board[t][d] == c)
                break;
        if (!has_hpeer(m, t, d) && board[t - 1][d] != '.') {
            badr = t; badc = d;
            --score;
        }
    }
    if (b < R - 1) {
        for (d = l; d <= r; ++d)
            if (board[b][d] == c)
                break;
        if (!has_hpeer(m, b, d) && board[b + 1][d] != '.') {
            badr = b; badc = d;
            --score;
        }
    }
    if (l > 0) {
        for (s = t; s <= b; ++s)
            if (board[s][l] == c)
                break;
        if (!has_vpeer(m, s, l) && board[s][l - 1] != '.') {
            badr = s; badc = l;
            --score;
        }
    }
    if (r < C - 1) {
        for (s = t; s <= b; ++s)
            if (board[s][r] == c)
                break;
        if (!has_vpeer(m, s, r) && board[s][r + 1] != '.') {
            badr = s; badc = r;
            --score;
        }
    }

    if (score < 3)
        return false;
    if (score == 3) {
        if (l < badc) --score;
        if (badc < r) --score;
        if (t < badr) --score;
        if (badr < b) --score;
        if (score)
            return false;
    }

    topleft[c] = {t, l};
    return true;
}

static void get_layers(const std::map<char, unsigned int>& covers) {
    if (covers.empty())
        task_out << ans << '\n';
    else {
        const auto pos = ans.size() - covers.size();
        for (auto it = covers.begin(); it != covers.end(); ++it)
            if (it->second == 0) {
                auto copy = covers;
                for (auto& d : copy)
                    d.second &= ~charbit(it->first);
                copy.erase(it->first);
                ans[pos] = it->first;
                get_layers(copy);
            }
    }
}

/**
 * @brief There is a simpler solution on USACO but is it correct?
 */
int main(int, char**)
{
    task_in >> R >> C;
    board.resize(R);
    for (auto& r : board)
        task_in >> r;

    // BUILD A MAP
    N.assign(R, std::vector<unsigned int>(C));
    E = S = W = N;
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c) {
            if (c) {
                W[r][c] = W[r][c - 1];
                const char w = board[r][c - 1];
                if (w != '.') W[r][c] |= charbit(w);
            }
            if (r) {
                N[r][c] = N[r - 1][c];
                const char n = board[r - 1][c];
                if (n != '.') N[r][c] |= charbit(n);
            }

            const char cell = board[r][c];
            if (cell == '.') continue;
            covers.try_emplace(cell, 0); // first occurence
            if (left.find(cell) == left.end()) { // first occurence
                bottom[cell] = top[cell] = r;
                left[cell] = right[cell] = c;
            } else {
                left[cell] = std::min(left[cell], c);
                right[cell] = std::max(right[cell], c);
                top[cell] = std::min(top[cell], r);
                bottom[cell] = std::max(bottom[cell], r);
            }
        }
    for (int r = R - 1; ~r; --r)
        for (int c = C - 1; ~c; --c) {
            if (c < C - 1) {
                E[r][c] = E[r][c + 1];
                const char e = board[r][c + 1];
                if (e != '.') E[r][c] |= charbit(e);
            }
            if (r < R - 1) {
                S[r][c] = S[r + 1][c];
                const char s = board[r + 1][c];
                if (s != '.') S[r][c] |= charbit(s);
            }
        }

    // IDENTIFY RECTANGLES
    for (auto& k : covers)
        if (identify_rectangle(k.first)) {
            const char cell = k.first;
            const auto cur = charbit(cell);
            const auto& [r, c] = topleft[cell];
            const auto rh = r + bottom[cell] - top[cell];
            const auto cw = c + right[cell] - left[cell];
            for (int d = c; d <= cw; ++d)
                if (board[r][d] != cell)
                    covers[board[r][d]] |= cur;
            for (int d = c; d <= cw; ++d)
                if (board[rh][d] != cell)
                    covers[board[rh][d]] |= cur;
            for (int s = r; s <= rh; ++s)
                if (board[s][c] != cell)
                    covers[board[s][c]] |= cur;
            for (int s = r; s <= rh; ++s)
                if (board[s][cw] != cell)
                    covers[board[s][cw]] |= cur;
        }

    ans.resize(covers.size());
    get_layers(covers);
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 frameup.cpp -o frameup.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address frameup.cpp -o frameup

Input:

9 8
.CCC....
ECBCBB..
DCBCDB..
DCCC.B..
D.B.ABAA
D.BBBB.A
DDDDAD.A
E...AAAA
EEEEEE..

Output:


*/

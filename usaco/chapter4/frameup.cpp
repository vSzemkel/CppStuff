/*
ID: marcin.8
LANG: C++17
TASK: frameup
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=GVqXl5lUbGQ&S=frameup
*/

#include <algorithm>
#include <assert.h>
#include <fstream>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

std::ifstream task_in("frameup.in");
std::ofstream task_out("frameup.out");

int R, C;
std::vector<std::string> board, initial;
std::map<char, int> covers; // {char, charbits covered}
std::unordered_map<char, std::pair<int, int>> topleft; // {char, top-left rectangle corner}
std::unordered_map<char, int> right, top, left, bottom; // {char, known extent position}
std::vector<std::vector<int>> N, E, S, W; // neibors on direction

inline static unsigned int charbit(const char c) {
    return 1 << (c - 'A');
}

inline static bool has_hpeer(const char x, const int r, const int c) {
    const auto b = charbit(x);
    return (W[r][c] & b) || (E[r][c] & b);
}

inline static bool has_vpeer(const char x, const int r, const int c) {
    const auto b = charbit(x);
    return (S[r][c] & b) || (N[r][c] & b);
}

static bool is_above(const char u, const char l) {
    if (topleft.count(l) == 0)
        return true; // inconclusive
    const auto& [r, c] = topleft[l];
    const auto rh = r + bottom[l] - top[l];
    const auto cw = c + right[l] - left[l];
    for (int d = c; d <= cw; ++d)
        if (initial[r][d] == u || initial[rh][d] == u) return true;
    for (int s = r; s <= rh; ++s)
        if (initial[s][c] == u || initial[s][cw] == u) return true;
    return false;
}

static bool identify_rectangle(const char c) {
    const auto r = right[c], t = top[c], l = left[c], b = bottom[c];
    const auto h = b - t, w = r - l;
    if (h < 2 || w < 2)
        return false;

    int d, s, badr{}, badc{}, score{4};
    if (t > 0) {
        for (d = l; d <= r; ++d)
            if (initial[t][d] == c)
                break;
        if (!has_hpeer(c, t, d) && initial[t - 1][d] != '.') {
            badr = t; badc = d;
            --score;
        }
    }
    if (b < R - 1) {
        for (d = l; d <= r; ++d)
            if (initial[b][d] == c)
                break;
        if (!has_hpeer(c, b, d) && initial[b + 1][d] != '.') {
            badr = b; badc = d;
            --score;
        }
    }
    if (l > 0) {
        for (s = t; s <= b; ++s)
            if (initial[s][l] == c)
                break;
        if (!has_vpeer(c, s, l) && initial[s][l - 1] != '.') {
            badr = s; badc = l;
            --score;
        }
    }
    if (r < C - 1) {
        for (s = t; s <= b; ++s)
            if (initial[s][r] == c)
                break;
        if (!has_vpeer(c, s, r) && initial[s][r + 1] != '.') {
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

static bool try_cover(const char top, const char bottom) {
    if (top == '.' || bottom == '.' || top == bottom)
        return false;
    if (covers[bottom] & charbit(top))
        return false;

    covers[top] |= charbit(bottom);
    return true;
}

static void get_layers(const std::string& prefix, const std::map<char, int>& covers) {
    if (covers.empty())
        task_out << prefix << '\n';
    else {
        auto it = covers.begin();
        while (true) {
            it = std::find_if(it, covers.end(), [&](const auto& c){ return c.second == 0; });
            if (it == covers.end())
                break;
            auto copy = covers;
            for (auto& d : copy)
                d.second &= ~charbit(it->first);
            copy.erase(it->first);
            get_layers(prefix + it->first, copy);
            ++it;
        }
    }
}

/**
 * @brief Stages 0 and 4 are sufficient. Others come from solutionprototypes
 */
int main(int, char**)
{
    task_in >> R >> C;
    board.resize(R);
    for (auto& r : board)
        task_in >> r;

    // STAGE 0 - build a map
    N.assign(R, std::vector<int>(C));
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

    // STAGE 1 - CERTEN HITS
    // determine covering relation
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c) {
            const char cell = board[r][c];
            if (cell == '.') continue;
            const auto cur = charbit(cell);
            const auto notcur = ~cur;

            int e = c + 1;
            while (e < C && board[r][e] == cell)
                ++e;
            if (c + 1 < e && e < C) { // found segment
                if (E[r][e] & cur) do {
                    try_cover(board[r][e], cell);
                    ++e;
                } while (e < C && (E[r][e] & cur));
            }

            int w = c - 1;
            while (~w && board[r][w] == cell)
                --w;
            if (w < c - 1 && ~w) {
                if (W[r][w] & cur) do {
                    try_cover(board[r][w], cell);
                    --w;
                } while (~w && (W[r][w] & cur));
            }

            int s = r + 1;
            while (s < R && board[s][c] == cell)
                ++s;
            if (r + 1 < s && s < R) {
                if (S[s][c] & cur) do {
                    try_cover(board[s][c], cell);
                    ++s;
                } while (s < R && (S[s][c] & cur));
            }
            
            int n = r - 1;
            while (~n && board[n][c] == cell)
                --n;
            if (n < r - 1 && ~n) {
                if (N[n][c] & cur) do {
                    try_cover(board[n][c], cell);
                    --n;
                } while (~n && (N[n][c] & cur));
            }

            // check if [r][c] is corner covering another corner
            if ((r < R - 1 && c < C - 1 && cell == board[r + 1][c] && cell == board[r][c + 1])
             || (r > 0 && c < C - 1 && cell == board[r - 1][c] && cell == board[r][c + 1])
             || (r < R - 1 && c > 0 && cell == board[r + 1][c] && cell == board[r][c - 1])
             || (r > 0 && c > 0 && cell == board[r - 1][c] && cell == board[r][c - 1]))
            {
                auto ne = N[r][c] & E[r][c] & notcur;
                if (ne) {
                    for (char x = 'A'; ne; ne >>= 1, ++x)
                        if (ne & 1) {
                            int cnt{0}, s{r - 1}, d{c + 1};
                            while (~s) {
                                if (board[s][c] == x) ++cnt;
                                --s;
                            }
                            while (d < C) {
                                if (board[r][d] == x) ++cnt;
                                ++d;
                            }
                            if (cnt > 2 || (c == 0 && r == R - 1))
                                try_cover(cell, x);
                        }
                }

                auto es = E[r][c] & S[r][c] & notcur;
                if (es) {
                    for (char x = 'A'; es; es >>= 1, ++x)
                        if (es & 1) {
                            int cnt{0}, s{r + 1}, d{c + 1};
                            while (s < R) {
                                if (board[s][c] == x) ++cnt;
                                ++s;
                            }
                            while (d < C) {
                                if (board[r][d] == x) ++cnt;
                                ++d;
                            }
                            if (cnt > 2 || (c == 0 && r == 0))
                                try_cover(cell, x);
                        }
                }

                auto sw = S[r][c] & W[r][c] & notcur;
                if (sw) {
                    for (char x = 'A'; sw; sw >>= 1, ++x)
                        if (sw & 1) {
                            int cnt{0}, s{r + 1}, d{c - 1};
                            while (s < R) {
                                if (board[s][c] == x) ++cnt;
                                ++s;
                            }
                            while (~d) {
                                if (board[r][d] == x) ++cnt;
                                --d;
                            }
                            if (cnt > 2 || (c == C - 1 && r == 0))
                                try_cover(cell, x);
                        }
                }

                auto wn = W[r][c] & N[r][c] & notcur;
                if (wn) {
                    for (char x = 'A'; wn; wn >>= 1, ++x)
                        if (wn & 1) {
                            int cnt{0}, s{r - 1}, d{c - 1};
                            while (~s) {
                                if (board[s][c] == x) ++cnt;
                                --s;
                            }
                            while (~d) {
                                if (board[r][d] == x) ++cnt;
                                --d;
                            }
                            if (cnt > 2 || (c == C - 1 && r == R - 1))
                                try_cover(cell, x);
                        }
                }
            }
        }

    // STAGE 2 - UNCERTEN HITS
    // search for uncovered rectangles this destroys the board
    initial = board;
    while (true) {
        bool found{false};
        for (int r = 0; r < R - 2; ++r)
            for (int c = 0; c < C - 2; ++c) {
                const char tl = board[r][c];
                if (tl == '.') continue;
                const auto rh = r + bottom[tl] - top[tl];
                const auto cw = c + right[tl] - left[tl];
                // check corners
                if (rh >= R || cw >= C) continue;
                const char tr = board[r][cw];
                if (tr != tl) continue;
                const char bl = board[rh][c];
                if (bl != tl) continue;
                const char br = board[rh][cw];
                if (br != tl) continue;
                topleft[tl] = {r, c};
                // check edges
                int d{c + 1};
                while (d <= cw && (board[r][d] == tl || (covers[tl] & charbit(board[r][d])))) ++d;
                if (d - 1 != cw) continue;
                d = c + 1;
                while (d <= cw && (board[rh][d] == tl || (covers[tl] & charbit(board[rh][d])))) ++d;
                if (d - 1 != cw) continue;
                int s{r + 1};
                while (s <= rh && (board[s][c] == tl || (covers[tl] & charbit(board[s][c])))) ++s;
                if (s - 1 != rh) continue;
                s = r + 1;
                while (s <= rh && (board[s][cw] == tl || (covers[tl] & charbit(board[s][cw])))) ++s;
                if (s - 1 != rh) continue;
                found = true;
                // clear out with dots
                d = c;
                while (d <= cw) {
                    board[r][d] = '.';
                    board[rh][d++] = '.';
                }
                s = r;
                while (s <= rh) {
                    board[s][c] = '.';
                    board[s++][cw] = '.';
                }
                // repaint colinear edges
                if (0 < c) {
                    const auto ltcan = board[r][c - 1];
                    if (ltcan != '.' && has_hpeer(ltcan, r, c - 1)) {
                        const auto rb = std::min(cw, right[ltcan]);
                        if (c <= rb)
                            try_cover(tl, ltcan);
                        for (int d = c; d <= rb; ++d)
                            board[r][d] = ltcan;
                    }
                    const auto lbcan = board[rh][c - 1];
                    if (lbcan != '.' && has_hpeer(lbcan, rh, c - 1)) {
                        const auto rb = std::min(cw, right[lbcan]);
                        if (c <= rb)
                            try_cover(tl, lbcan);
                        for (int d = c; d <= rb; ++d)
                            board[rh][d] = lbcan;
                    }
                }
                if (cw < C - 1) {
                    const auto rtcan = board[r][cw + 1];
                    if (rtcan != '.' && has_hpeer(rtcan, r, cw + 1)) {
                        const auto lb = std::max(c, left[rtcan]);
                        if (lb <= cw)
                            try_cover(tl, rtcan);
                        for (int d = cw; lb <= d; --d)
                            board[r][d] = rtcan;
                    }
                    const auto rbcan = board[rh][cw + 1];
                    if (rbcan != '.' && has_hpeer(rbcan, rh, cw + 1)) {
                        const auto lb = std::max(c, left[rbcan]);
                        if (lb <= cw)
                            try_cover(tl, rbcan);
                        for (int d = cw; lb <= d; --d)
                            board[rh][d] = rbcan;
                    }
                }
                if (0 < r) {
                    const auto ltcan = board[r - 1][c];
                    if (ltcan != '.' && has_vpeer(ltcan, r - 1, c)) {
                        const auto bb = std::min(rh, bottom[ltcan]);
                        if (r <= bb)
                            try_cover(tl, ltcan);
                        for (int s = r; s <= bb; ++s)
                            board[s][c] = ltcan;
                    }
                    const auto rtcan = board[r - 1][cw];
                    if (rtcan != '.' && has_vpeer(rtcan, r - 1, cw)) {
                        const auto bb = std::min(rh, bottom[rtcan]);
                        if (r <= bb)
                            try_cover(tl, rtcan);
                        for (int s = r; s <= bb; ++s)
                            board[s][cw] = rtcan;
                    }
                }
                if (rh < R - 1) {
                    const auto lbcan = board[rh + 1][c];
                    if (lbcan != '.' && has_vpeer(lbcan, rh + 1, c)) {
                        const auto tb = std::max(r, top[lbcan]);
                        if (tb <= rh)
                            try_cover(tl, lbcan);
                        for (int s = rh; tb <= s; --s)
                            board[s][c] = lbcan;
                    }
                    const auto rbcan = board[rh + 1][cw];
                    if (rbcan != '.' && has_vpeer(rbcan, rh + 1, cw)) {
                        const auto tb = std::max(r, top[rbcan]);
                        if (tb <= rh)
                            try_cover(tl, rbcan);
                        for (int s = rh; tb <= s; --s)
                            board[s][cw] = rbcan;
                    }
                }
                // repaint uncovered edges
                if (0 < r && r < R - 1)
                    for (d = c + 1; d < cw; ++d) {
                        const char x = board[r - 1][d];
                        if (x != '.' && board[r + 1][d] == x) {
                            try_cover(tl, x);
                            board[r][d] = x;
                        }
                    }
                if (0 < rh && rh < R - 1)
                    for (d = c + 1; d < cw; ++d) {
                        const char x = board[rh - 1][d];
                        if (x != '.' && board[rh + 1][d] == x) {
                            try_cover(tl, x);
                            board[rh][d] = x;
                        }
                    }
                if (0 < c && c < C - 1)
                    for (s = r + 1; s < rh; ++s) {
                        const char x = board[s][c - 1];
                        if (x != '.' && board[s][c + 1] == x) {
                            try_cover(tl, x);
                            board[s][c] = x;
                        }
                    }
                if (0 < cw && cw < C - 1)
                    for (s = r + 1; s < rh; ++s) {
                        const char x = board[s][cw - 1];
                        if (x != '.' && board[s][cw + 1] == x) {
                            try_cover(tl, x);
                            board[s][cw] = x;
                        }
                    }
            }

        if (!found)
            break;
    }

    // STAGE 3 - REMOVE FALSE POSITIVES
    for (auto& c : covers) {
        char b = 'A';
        for (int m{1}, l{c.second}; l; l >>= 1, m <<= 1, ++b)
            if ((l & 1) && !is_above(c.first, b))
                c.second &= ~m;
    }

    // STAGE 4 - IDENTIFY HARD RECTANGLES
    for (auto& k : covers)
        if (topleft.count(k.first) == 0 && identify_rectangle(k.first)) {
            const int cell = k.first;
            const auto cur = charbit(cell);
            const auto& [r, c] = topleft[cell];
            const auto rh = r + bottom[cell] - top[cell];
            const auto cw = c + right[cell] - left[cell];
            for (int d = c; d <= cw; ++d)
                if (initial[r][d] != cell)
                    covers[initial[r][d]] |= cur;
            for (int d = c; d <= cw; ++d)
                if (initial[rh][d] != cell)
                    covers[initial[rh][d]] |= cur;
            for (int s = r; s <= rh; ++s)
                if (initial[s][c] != cell)
                    covers[initial[s][c]] |= cur;
            for (int s = r; s <= rh; ++s)
                if (initial[s][cw] != cell)
                    covers[initial[s][cw]] |= cur;
        }

    get_layers("", covers);
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

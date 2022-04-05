/*
ID: marcin.8
LANG: C++
TASK: frameup
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=GVqXl5lUbGQ&S=frameup
*/

#include <algorithm>
#include <array>
#include <assert.h>
#include <bitset>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <filesystem>
#include <fstream>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <stdlib.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <vector>

std::ifstream task_in("frameup.in");
std::ofstream task_out("frameup.out");

int R, C;
std::vector<std::string> board;
std::vector<std::vector<int>> N, E, S, W; // neibors on direction

static unsigned int charbit(const char c) {
    return 1 << (c - 'A');
}

static std::vector<std::string> get_layers(const std::unordered_map<char, int>& covers) {
    std::vector<std::string> ret;
    for (const auto& c : covers)
        if (c.second == 0) {
            auto copy = covers;
            copy.erase(c.first);
            if (copy.empty())
                ret.push_back(std::string(1, c.first));
            else {
                for (auto& d : copy)
                    d.second &= ~charbit(c.first);
                const auto tails = get_layers(copy);
                for (const auto& t : tails)
                    ret.push_back(c.first + t);
            }
        }

    return ret;
}

int main(int, char**)
{
    task_in >> R >> C;
    board.resize(R);
    for (auto& r : board)
        task_in >> r;

    // build a map
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

    // determine covering relation
    std::unordered_map<char, int> covers; // {char, charbits covered}
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c) {
            const char cell = board[r][c];
            if (cell == '.') continue;
            if (covers.find(cell) == covers.end())
                covers[cell] = 0;
            //covers.try_emplace(cell, 0);
            const auto cur = charbit(cell);
            const auto notcur = ~cur;

            int e = c + 1;
            while (e < C && board[r][e] == cell)
                ++e;
            if (c + 1 < e && e < C) { // found segment
                if (E[r][e] & cur) do {
                    if (board[r][e] != cell)
                        covers[board[r][e]] |= cur;
                    ++e;
                } while (e < C && (E[r][e] & cur));
            }

            int w = c - 1;
            while (~w && board[r][w] == cell)
                --w;
            if (w < c - 1 && ~w) {
                if (W[r][w] & cur) do {
                    if (board[r][w] != cell)
                        covers[board[r][w]] |= cur;
                    --w;
                } while (~w && (W[r][w] & cur));
            }

            int s = r + 1;
            while (s < R && board[s][c] == cell)
                ++s;
            if (r + 1 < s && s < R) {
                if (S[s][c] & cur) do {
                    if (board[s][c] != cell)
                        covers[board[s][c]] |= cur;
                    ++s;
                } while (s < R && (S[s][c] & cur));
            }
            
            int n = r - 1;
            while (~n && board[n][c] == cell)
                --n;
            if (n < r - 1 && ~n) {
                if (N[n][c] & cur) do {
                    if (board[n][c] != cell)
                        covers[board[n][c]] |= cur;
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
                                covers[cell] |= charbit(x);
                        }
                } else if (r > 0 && c < C - 1 && board[r - 1][c] == cell && board[r][c + 1] == cell && board[0][c] == cell && board[r][C - 1] == cell) {
                    for (int d = c + 1; d < C; ++d) {
                        const char x = board[0][d];
                        if (x != '.' && x != cell)
                            covers[x] |= cur;
                    }
                    for (int s = r - 1; ~s; --s) {
                        const char x = board[s][C - 1];
                        if (x != '.' && x != cell)
                            covers[x] |= cur;
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
                                covers[cell] |= charbit(x);
                        }
                } else if (r < R - 1 && c < C - 1 && board[r + 1][c] == cell && board[r][c + 1] == cell && board[R - 1][c] == cell && board[r][C - 1] == cell) {
                    for (int d = c + 1; d < C; ++d) {
                        const char x = board[R - 1][d];
                        if (x != '.' && x != cell)
                            covers[x] |= cur;
                    }
                    for (int s = r + 1; s < R; ++s) {
                        const char x = board[s][C - 1];
                        if (x != '.' && x != cell)
                            covers[x] |= cur;
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
                                covers[cell] |= charbit(x);
                        }
                } else if (r < R - 1 && c > 0 && board[r + 1][c] == cell && board[r][c - 1] == cell && board[R - 1][c] == cell && board[r][0] == cell) {
                    for (int d = c - 1; ~d; --d) {
                        const char x = board[R - 1][d];
                        if (x != '.' && x != cell)
                            covers[x] |= cur;
                    }
                    for (int s = r + 1; s < R; ++s) {
                        const char x = board[s][0];
                        if (x != '.' && x != cell)
                            covers[x] |= cur;
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
                                covers[cell] |= charbit(x);
                        }
                } else if (r > 0 && c > 0 && board[r - 1][c] == cell && board[r][c - 1] == cell && board[0][c] == cell && board[r][0] == cell) {
                    for (int d = c - 1; ~d; --d) {
                        const char x = board[0][d];
                        if (x != '.' && x != cell)
                            covers[x] |= cur;
                    }
                    for (int s = r - 1; ~s; --s) {
                        const char x = board[s][0];
                        if (x != '.' && x != cell)
                            covers[x] |= cur;
                    }
                }
            }
        }

    // search for uncovered rectangles this destroys the board
    while (true) {
        bool found{false};
        for (int r = 0; r < R - 2; ++r)
            for (int c = 0; c < C - 2; ++c) {
                const char tl = board[r][c];
                if (tl == '.') continue;
                for (int h = 2; r + h < R; ++h)
                    for (int w = 2; c + w < C; ++w) {
                        const auto rh = r + h;
                        const auto cw = c + w;
                        // check corners
                        const char tr = board[r][cw];
                        if (tr != tl) continue;
                        const char bl = board[rh][c];
                        if (bl != tl) continue;
                        const char br = board[rh][cw];
                        if (br != tl) continue;
                        // check edges
                        int d{c + 1};
                        while (d < C && board[r][d] == tl) ++d;
                        if (d - 1 != cw) continue;
                        d = c + 1;
                        while (d < C && board[rh][d] == tl) ++d;
                        if (d - 1 != cw) continue;
                        int s{r + 1};
                        while (s < R && board[s][c] == tl) ++s;
                        if (s - 1 != rh) continue;
                        s = r + 1;
                        while (s < R && board[s][cw] == tl) ++s;
                        if (s - 1 != rh) continue;
                        found = true;
                        // clear out with dots
                        d = c;
                        while (d <= c + w) {
                            board[r][d] = '.';
                            board[rh][d++] = '.';
                        }
                        s = r;
                        while (s <= rh) {
                            board[s][c] = '.';
                            board[s++][cw] = '.';
                        }
                        // repaint uncovered edges
                        if (0 < r && r < R - 1)
                            for (d = c + 1; d < cw; ++d) {
                                const char x = board[r - 1][d];
                                if (x != '.' && board[r + 1][d] == x) {
                                    covers[tl] |= charbit(x);
                                    board[r][d] = x;
                                }
                            }
                        if (0 < rh && rh < R - 1)
                            for (d = c + 1; d < cw; ++d) {
                                const char x = board[rh - 1][d];
                                if (x != '.' && board[rh + 1][d] == x) {
                                    covers[tl] |= charbit(x);
                                    board[rh][d] = x;
                                }
                            }
                        if (0 < c && c < C - 1)
                            for (s = r + 1; s < rh; ++s) {
                                const char x = board[s][c - 1];
                                if (x != '.' && board[s][c + 1] == x) {
                                    covers[tl] |= charbit(x);
                                    board[s][c] = x;
                                }
                            }
                        if (0 < cw && cw < C - 1)
                            for (s = r + 1; s < rh; ++s) {
                                const char x = board[s][cw - 1];
                                if (x != '.' && board[s][cw + 1] == x) {
                                    covers[tl] |= charbit(x);
                                    board[s][cw] = x;
                                }
                            }
                        // repaint colinear edges
                        if (0 < c && cw < C - 1) {
                            const auto tcan = board[r][c - 1];
                            if (tcan != '.' && tcan == board[r][cw + 1]) {
                                covers[tl] |= charbit(tcan);
                                for (int d = c; d <= cw; ++d)
                                    board[r][d] = tcan;
                            }
                            const auto bcan = board[rh][c - 1];
                            if (bcan != '.' && bcan == board[rh][cw + 1]) {
                                covers[tl] |= charbit(bcan);
                                for (int d = c; d <= cw; ++d)
                                    board[rh][d] = bcan;
                            }
                        }
                        if (0 < r && rh < R - 1) {
                            const auto lcan = board[r - 1][c];
                            if (lcan != '.' && lcan == board[rh + 1][c]) {
                                covers[tl] |= charbit(lcan);
                                for (int s = r; s <= rh; ++s)
                                    board[s][c] = lcan;
                            }
                            const auto rcan = board[r - 1][cw];
                            if (rcan != '.' && rcan == board[rh + 1][cw]) {
                                covers[tl] |= charbit(rcan);
                                for (int s = r; s <= rh; ++s)
                                    board[s][cw] = rcan;
                            }
                        }
                    }
            }

        if (!found)
            break;
    }

    covers.erase('.');
    auto ans = get_layers(covers);
    std::sort(ans.begin(), ans.end());
    for (const auto& a : ans)
        task_out << a << '\n';
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

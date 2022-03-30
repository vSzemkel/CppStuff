// board with walls

#include <array>
#include <vector>

constexpr int N{0}, E{1}, S{2}, W{3};
std::array<std::vector<int>, 4> wall;

    wall.fill(std::vector<int>(size, 1)); // init is outer wall status
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c) {
            const auto cell = r * C + c;
            if (board[r][c] == '#') { // wall mapping
                wall[N][cell] = wall[W][cell] = wall[S][cell] = wall[E][cell] = 0;
                continue;
            }
            if (c > 0)
                wall[W][cell] = wall[W][cell - 1] + 1;
            if (r > 0)
                wall[N][cell] = wall[N][cell - C] + 1;
        }
    for (int r = R - 1; ~r; --r)
        for (int c = C - 1; ~c; --c) {
            const auto cell = r * C + c;
            if (board[r][c] == '#')
                continue;
            if (c < C - 1)
                wall[E][cell] = wall[E][cell + 1] + 1;
            if (r < R - 1)
                wall[S][cell] = wall[S][cell + C] + 1;
        }

************************************************************

    N.assign(R, std::vector<int>(C));
    E = S = W = N;
    for (int r = 1; r < R; ++r)
        for (int c = 1; c < C; ++c) {
            W[r][c] = W[r][c - 1];
            const char w = board[r][c - 1];
            if (w != '.')
                W[r][c] |= 1 << (w - 'A');
            N[r][c] = N[r - 1][c];
            const char n = board[r - 1][c];
            if (n != '.')
                N[r][c] |= 1 << (n - 'A');
        }
    for (int r = R - 2; ~r; --r)
        for (int c = C - 2; ~c; --c) {
            E[r][c] = E[r][c + 1];
            const char e = board[r][c + 1];
            if (e != '.')
                E[r][c] |= 1 << (e - 'A');
            S[r][c] = S[r + 1][c];
            const char s = board[r + 1][c];
            if (s != '.')
                S[r][c] |= 1 << (s - 'A');
        }

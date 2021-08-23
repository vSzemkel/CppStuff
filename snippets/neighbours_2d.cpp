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
    for (int r = R - 1; r >= 0; --r)
        for (int c = C - 1; c >= 0; --c) {
            const auto cell = r * C + c;
            if (board[r][c] == '#')
                continue;
            if (c < C - 1)
                wall[E][cell] = wall[E][cell + 1] + 1;
            if (r < R - 1)
                wall[S][cell] = wall[S][cell + C] + 1;
        }

/*
ID: marcin.8
LANG: C++
TASK: wissqu
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=d7HUb5da4a6&S=wissqu
*/

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

std::ifstream task_in("wissqu.in");
std::ofstream task_out("wissqu.out");

static constexpr const int SZ = 4;
static constexpr const int BREEDS = 5;
static constexpr const int SZ2 = SZ * SZ;

struct move_t {
    int r, c;
    char breed;
};

bool found{};
int64_t count;                      // number of solutions
std::vector<move_t> solution;       // lexicographically smallest solution
std::vector<bool> visited(SZ2);     // progress map
std::array<int, BREEDS> calves;     // how many cows of particular breed still not moved to yearling pasture
std::array<int, BREEDS> breeds;     // for each breed bitmap of allowed positions
std::vector<std::string> board(SZ); // yearling pasture state

void search(const int pending);

auto sync_enter(const int row, const int col)
{
    auto& cur_breed_map = breeds[board[row][col] - 'A'];
    const auto old = cur_breed_map;
    for (int r = row - 1; r <= row + 1; ++r)
        for (int c = col - 1; c <= col + 1; ++c)
            if (0 <= r && 0 <= c && r < SZ && c < SZ) {
                const int mask = 1 << (r * SZ + c);
                cur_breed_map  &= ~mask;
            }
    return old;
}

auto sync_leave(const char breed)
{
    auto& cur_breed_map = breeds[breed - 'A'];
    const auto old = cur_breed_map;
    cur_breed_map = -1;
    for (int r = 0; r < SZ; ++r)
        for (int c = 0; c < SZ; ++c)
            if (board[r][c] == breed)
                sync_enter(r, c);
    return old;
}

void arrange(const int pos, char next, const int pending)
{
    const auto [r, c] = std::div(pos, SZ);
    auto& b = board[r][c];
    std::swap(b, next);
    const auto enter_org = sync_enter(r, c);
    const auto leave_org = sync_leave(next);
    visited[pos] = true;
    if (!found) {
        solution.emplace_back(move_t{r, c, b});
        search(pending);
        solution.pop_back();
    } else
        search(pending);

    visited[pos] = false;
    std::swap(b, next);
    breeds[next - 'A'] = enter_org;
    breeds[b - 'A'] = leave_org;
}

void search(const int pending)
{
    if (!pending) {
        ++count;
        if (!found) {
            found = true;
            for (const auto& s : solution)
                task_out << s.breed << ' ' << s.r + 1 << ' ' << s.c + 1 << '\n';
        }
    } else {
        for (int b = 0; b < BREEDS; ++b)
            if (calves[b]) {
                auto& breed = breeds[b];
                if (breed != 0)
                    for (int pos = 0; pos < SZ2; ++pos)
                        if (!visited[pos] && (breed & (1 << pos))) {
                            --calves[b];
                            arrange(pos, 'A' + b, pending - 1);
                            ++calves[b];
                        }
            }
    }
}

int main(int, char**)
{
    for (auto& r : board)
        task_in >> r;

    calves.fill(3);
    for (char b = 'A', z = BREEDS; z; ++b, --z)
        sync_leave(b);

    const char init{'D'};
    const auto& d = breeds[init - 'A'];
    for (int pos = 0; pos < SZ2; ++pos)
        if (d & (1 << pos)) 
            arrange(pos, init, SZ2 - 1);

    task_out << count << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 wissqu.cpp -o wissqu.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address wissqu.cpp -o wissqu

Run:
wissqu.exe && type wissqu.out

Input:

ABAC
DCDE
BEBC
CADE

Output:

D 4 1
C 4 2
A 3 1
A 3 3
B 2 4
B 3 2
B 4 4
E 2 1
E 2 3
D 1 4
D 2 2
C 1 1
C 1 3
A 1 2
E 4 3
D 3 4
14925

*/

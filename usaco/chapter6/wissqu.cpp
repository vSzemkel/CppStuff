/*
ID: marcin.8
LANG: C++
TASK: wissqu
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=d7HUb5da4a6&S=wissqu
*/

#include <algorithm>
#include <array>
#include <cassert>
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
int64_t count;
std::vector<move_t> solution;
std::array<int, BREEDS> calves; 
std::array<std::vector<bool>, BREEDS> breeds;
std::vector<std::string> board(SZ);

void report()
{
}

void map_possible_local(const int row, const int col)
{
    auto& cur_breed_map = breeds[board[row][col] - 'A'];
    for (int r = row - 1; r <= row + 1; ++r)
        for (int c = col - 1; c <= col + 1; ++c)
            if (0 <= r && 0 <= c && r < SZ && c < SZ)
                cur_breed_map[r * SZ + c] = true;
}

void map_possible()
{
    for (auto& b : breeds)
        b.assign(SZ2, true);

    for (int r = 0; r < SZ; ++r)
        for (int c = 0; c < SZ; ++c)
            map_possible_local(r, c);
}

void search(const int pending)
{
    if (!pendinf) {
        if (!found) {
            found = true;
            for (const auto& s : solution)
                task_out << s.breed << ' ' << s.r + 1 << ' ' << s.c + 1 << '\n';
        }
    } else {

    }
}

int main(int, char**)
{
    for (auto& r : board)
        task_in >> r;

    calves.fill(3);
    char init{'D'};
    const auto& d = breeds[init - 'A'];
    for (int pos = 0; pos < SZ2; ++pos)
        if (d[pos]) {
            auto& b = board[pos / SZ][pos % SZ];
            std::swap(b, init);
            search(SZ2 - 1);
            std::swap(b, init);
        }

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

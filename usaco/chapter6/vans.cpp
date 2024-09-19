/*
ID: marcin.8
LANG: C++
TASK: vans
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=0nOb9wTUsiE&S=vans
*/

#include <iostream>
#include <fstream>
#include <vector>

std::ifstream task_in("vans.in");
std::ofstream task_out("vans.out");

union state_t {
    struct {
        int16_t column;
        int8_t row;
        int8_t avoid;
    };
    int32_t state;
} ;

state_t final;
int SZ, ALL, ans{}, count{1};
std::vector<bool> visited;
constexpr const int /*N{0}, E{1},*/ S{2}, W{3};
constexpr const int8_t DR[] = {-1, 0, +1, 0};
constexpr const int8_t DC[] = {0, +1, 0, -1};

void ride(const state_t s)
{
    if (count == ALL && s.state == final.state) {
        ++ans;
        return;
    }

    ++count;
    for (int z = 3, dir = (s.avoid + 1) % 4; z; --z, dir = (dir + 1) % 4) {
        const int8_t nr = s.row + DR[dir];
        const int16_t nc = s.column + DC[dir];
        const auto vertex = nr * SZ + nc;
        if (0 <= nr && nr < 4 && 0 <= nc && nc < SZ && !visited[vertex]) {
            state_t next{ .row = nr, .column = nc, .avoid = static_cast<int8_t>((dir + 2) % 4) };
            visited[vertex] = true;
            ride(next);
            visited[vertex] = false;
        }
    }
    --count;
}

int main(int, char**)
{
    task_in >> SZ;
    if (SZ > 1) {
        ALL = 4 * SZ;
        visited.assign(ALL, false);
        visited[1] = true;
        state_t init{ .row = 0, .column = 1, .avoid = W };
        final = state_t{ .row = 0, .column = 0, .avoid = S };
        ride(init);
    }

    task_out << 2 * ans << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 vans.cpp -o vans.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address vans.cpp -o vans

Run:
vans.exe && type vans.out

Input:

10 

Output:

3034

*/

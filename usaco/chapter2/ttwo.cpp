/*
ID: marcin.8
LANG: C++
TASK: ttwo
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=u1s4VwnUpTj&S=ttwo
*/

#include <fstream>
#include <string>

std::ifstream task_in("ttwo.in");
std::ofstream task_out("ttwo.out");

constexpr const int size = 10;
const char T = '*'; // taken
const char F = 'F'; // farmer
const char C = 'C'; // cows

std::string grid[size];

struct state_t {
    int row, col;
    int dir{0}; // 0 == N; 1 == E; 2 == S; 3 == W
    bool operator==(const state_t& other) const { return row == other.row && col == other.col; }
    void advance() {
        const int next_r = row + d4r[dir];
        const int next_c = col + d4c[dir];
        if (next_r < 0 || next_r == size || next_c < 0 || next_c == size || grid[next_r][next_c] == T)
            dir = (dir + 1) % 4;
        else {
            row = next_r;
            col = next_c;
        }
    }

  private:
    static constexpr const int d4c[4] = {0, 1, 0, -1}, d4r[4] = {-1, 0, 1, 0}; // 0 == N; 1 == E; 2 == S; 3 == W clockwise
};

int main(int, char**)
{
    for (auto& row : grid)
        task_in >> row;

    state_t farmer, cows;
    for (int r = 0; r < size; ++r)
        for (int c = 0; c < size; ++c) {
            if (grid[r][c] == F) {
                farmer.row = r;
                farmer.col = c;
                // there is no need for mark grid[r][c] = '.' as 'F' != '*'
            }
            if (grid[r][c] == C) {
                cows.row = r;
                cows.col = c;
            }
        }

    constexpr const int no_of_states = (4 * 10 * 10) * (4 * 10 * 10);
    for (int i = 0; i < no_of_states; ++i) {
        if (farmer == cows) {
            task_out << i << '\n';
            return 0;
        }
        farmer.advance();
        cows.advance();
    }

    task_out << "0\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 ttwo.cpp -o ttwo.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address ttwo.cpp -o ttwo

Input:


Output:


*/

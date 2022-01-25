/*
ID: marcin.8
LANG: C++
TASK: camelot
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=unnUOe16hgm&S=camelot
*/

#include <algorithm>
#include <array>
#include <assert.h>
#include <fstream>
#include <numeric>
#include <vector>

std::ifstream task_in("camelot.in");
std::ofstream task_out("camelot.out");

using pos_t = std::array<int, 2>;

constexpr int kd[6] = {0, 3, 2, 3, 2, 1}; // 3 * r + c

static int knight_distance(int r, int c) {
    int ret{0};
    while (r + c > 3) {
        if (r > c) {
            r -= 2;
            --c;
        } else {
            c -= 2;
            --r;
        }
        ++ret;
    }

    if (r <= c)
        return ret + kd[3 * r + c];

    return ret + kd[3 * c + r];

    /*if (c == 0) {
        if (r == 0) return 0;
        if (r == 1) return 3;
    } else if (c == 1) {
        if (r == 0) return 3;
        if (r == 1) return 2;
    } else { // c == 2
        if (r == 0) return 2;
        if (r == 1) return 1;
    }

    assert(knight_distance(6, 5) == 5);
    assert(knight_distance(5, 5) == 6);
    assert(knight_distance(5, 4) == 3);
    assert(knight_distance(5, 3) == 4);
    assert(knight_distance(4, 4) == 4);*/
}

int main(int, char**)
{
    int R, C;
    task_in >> R >> C;
    char r; int c;
    task_in >> c >> r;
    pos_t king = {r - 1, c - 'A'};
    std::vector<pos_t> knights;
    while (task_in.good()) {
        task_in >> c >> r;
        knights.push_back({r - 1, c - 'A'});
    }

    // take the king
    const auto it = std::min_element(knights.begin(), knights.end(), [&king](const pos_t& knight){
        const int dr = std::abs(king[0] - knight[0]);
        const int dc = std::abs(king[1] - knight[1]);
        return dr * dr + dc * dc;
    });
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 camelot.cpp -o camelot.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address camelot.cpp -o camelot

Input:


Output:


*/

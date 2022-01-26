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

struct pos_t : std::array<int, 2> {
    pos_t diff(const pos_t& other) const {
        pos_t k; k[0] = std::abs((*this)[0] - other[0]); k[1] = std::abs((*this)[1] - other[1]);
        return k;
        //return pos_t{std::abs((*this)[0] - other[0]), std::abs((*this)[1] - other[1])};
    }
};

constexpr int kd[6] = {0, 3, 2, 3, 2, 1}; // 3 * r + c

/**
 * @brief Move knight towards origin
 * 
 * @param out r closest position in 2x3 surroundings
 * @param out c closest position in 2x3 surroundings
 * @return int Number of moves
 */
static int distance(int& r, int& c) {
    int ret{0};
    while (r + c > 3) {
        if (r > c) {
            r -= 2;
            c ? --c : c = 1;
        } else {
            c -= 2;
            r ? --r : r = 1;
        }
        ++ret;
    }

    if (r < 0) r = 1;
    if (c < 0) c = 1;
    if (r == 3 || c == 3)
        r = c = 1;

    return ret;
}

static int knight_distance(int r, int c) {
    const int ret = distance(r, c);
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

/**
 * @brief Let the knight meet the king in minimal numer of moves
 * @param knight Knights position is set to meetpoint
 * @return int Number of moves
 */
static int king_knight_meeting(const pos_t& king, pos_t& knight) {
    const auto d = king.diff(knight);
    int r{d[0]}, c{d[1]};
    int ret = distance(r, c);
    const int sigr = r ? (knight[0] - king[0]) / r : 0;
    const int sigc = c ? (knight[1] - king[1]) / c : 0;

    knight[0] = king[0] + r * sigr;
    knight[1] = king[1] + c * sigc;

    return ret + std::max(r, c);

    /*pos_t king2{5, 5}, knight{8, 9};
    assert((king_knight_meeting(king2, knight) == 3 && knight == pos_t{6, 5}));*/
}

int main(int, char**)
{
    int R, C;
    task_in >> R >> C;
    const int SZ = R * C;
    int r; char c;
    task_in >> c >> r;
    //pos_t king = {r - 1, c[0] - 'A'};
    pos_t king; king[0] = r - 1; king[1] = c - 'A';
    std::vector<pos_t> knights;
    while (true) {
        task_in >> c >> r;
        pos_t k; k[0] = r - 1; k[1] = c - 'A';
        knights.push_back(k);
        if (!task_in.good())
            break;
        //knights.push_back({r - 1, c[0] - 'A'});
    };

    if (knights.empty()) {
        task_out << "0\n";
        return 0;
    }

    // take the king
    pos_t& driver = *std::min_element(knights.begin(), knights.end(), [&king](const pos_t& knight1, const pos_t& knight2){
        const pos_t d1 = king.diff(knight1);
        const pos_t d2 = king.diff(knight2);
        return d1[0] * d1[0] + d1[1] * d1[1] < d2[0] * d2[0] + d2[1] * d2[1];
    });

    int ans = 1e09;
    int meet_king = king_knight_meeting(king, driver);
    for (int pos = 0; pos < SZ; ++pos) {
        int can{meet_king};
        for (const auto& k : knights) {
            pos_t p; p[0] = pos / C; p[1] = pos % C;
            const auto d = k.diff(p);
            //const auto d = k.diff(pos_t{pos / C, pos % C});
            can += knight_distance(d[0], d[1]);
        }
        ans = std::min(ans, can);
    }

    task_out << ans << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 camelot.cpp -o camelot.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address camelot.cpp -o camelot

Input:


Output:


*/


#include <iostream>
#include <string>

// Expogo
// https://codingcompetitions.withgoogle.com/codejam/round/000000000019fef2/00000000002d5b62

bool g_rev_x, g_rev_y;

void solve(int x, int y) // O(log(x + y))
{
    g_rev_x = x < 0;
    g_rev_y = y < 0;

    if (g_rev_x) x *= -1;
    if (g_rev_y) y *= -1;

    int depth = 0;
    std::string path(31, '\0');
    while (((x + y) % 2) == 1) {
        char& dir = path[depth++];
        if (x % 2 == 1) {
            if ((((x - 1) / 2 + y / 2) % 2) == 1 || x + y == 1) {
                dir = 'E';
                --x;
            } else {
                dir = 'W';
                ++x;
            }
        } else {
            if ((((y - 1) / 2 + x / 2) % 2) == 1 || x + y == 1) {
                dir = 'N';
                --y;
            } else {
                dir = 'S';
                ++y;
            }
        }
        x >>= 1;
        y >>= 1;
    }

    if (x == 0 && y == 0)
        for (const char d : path) {
            if (d == 'W') { std::cout << (g_rev_x ? 'E' : 'W'); continue; }
            if (d == 'E') { std::cout << (g_rev_x ? 'W' : 'E'); continue; }
            if (d == 'N') { std::cout << (g_rev_y ? 'S' : 'N'); continue; }
            if (d == 'S') { std::cout << (g_rev_y ? 'N' : 'S'); continue; }
        }
    else
        std::cout << "IMPOSSIBLE";
}

void solve2(int x, int y) // O(x + y)
{
    g_rev_x = x < 0;
    g_rev_y = y < 0;
    if (g_rev_x) x *= -1;
    if (g_rev_y) y *= -1;

    // x + y <= sum
    // MX + MY == sum == 2**i - 1
    // MX & MY == 0
    // RX + LX == MX
    // RX - LX == x
    // RX & LX == 0
    // TY + BY == MY
    // TY - BY == y
    // TY & BY == 0

    int i{1};
    while (i <= x + y) i <<= 1;

    bool found{false};
    int RX{0}, LX{0}, TY{0}, BY{0}, sum = i - 1;
    for (int MX = x; MX <= sum - y; MX += 2) {
        int MY = sum - MX;
        if ((MX & MY) != 0) continue;
        RX = (MX + x) >> 1;
        LX = RX - x;
        TY = (MY + y) >> 1;
        BY = TY - y;
        if (LX >= 0 && BY >= 0 && (RX & LX) == 0 && (TY & BY) == 0 && ((RX + LX) & (TY + BY)) == 0 && (RX + LX + TY + BY == sum) && ((RX | LX | TY | BY) == sum)) {
            found = true;
            break;
        }
    }

    if (found)
        for (int i = 1; i <= sum; i <<= 1) {
            if (RX & i) { std::cout << (g_rev_x ? 'W' : 'E'); continue; }
            if (LX & i) { std::cout << (g_rev_x ? 'E' : 'W'); continue; }
            if (TY & i) { std::cout << (g_rev_y ? 'S' : 'N'); continue; }
            if (BY & i) { std::cout << (g_rev_y ? 'N' : 'S'); continue; }
        }
    else
        std::cout << "IMPOSSIBLE";
}

void fuzz()
{
    for (int x = -4; x < 6; ++x)
        for (int y = -4; y < 5; ++y) {
            std::cout << "\n" << x << ", " << y << ": "; solve(x, y);
            std::cout << "\n" << x << ", " << y << ": "; solve2(x, y);
        }
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    // fuzz(); return 0;
    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        int x, y;
        std::cin >> x >> y;
        std::cout << "Case #" << g << ": "; solve(x, y); std::cout << "\n";
    }
}

/*
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 expogo.cpp -o expogo.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 expogo.cpp -o expogo.o
expogo.exe < expogo.in

Input:

7
-4 1
2 3
-2 -3
3 0
-1 1
6 5
7 10

Output:

Case #1: SNW
Case #2: SEN
Case #3: NWS
Case #4: EE
Case #5: IMPOSSIBLE
Case #6: NWNE
Case #7: WSSEN

*/

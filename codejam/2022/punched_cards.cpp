
#include <iostream>
#include <string>

// Punched Cards
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000876ff1/0000000000a4621b

static void solve() {
    int R, C;
    std::cin >> R >> C;

    const int h = 2 * R + 1;
    const int w = 2 * C + 1;
    std::string border(w, '+');
    for (int i = 1; i < w; i += 2) border[i] = '-';
    std::string row(w, '|');
    for (int i = 1; i < w; i += 2) row[i] = '.';

    auto first_border = border;
    first_border[0] = first_border[1] = '.';
    auto first_row = row;
    first_row[0] = first_row[1] = '.';

    std::cout << '\n' << first_border << '\n';
    std::cout << first_row << '\n';
    for (int z = R - 1; z; --z) {
        std::cout << border << '\n';
        std::cout << row << '\n';
    }
    std::cout << border << '\n';
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 punched_cards.cpp -o punched_cards.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address punched_cards.cpp -o punched_cards

Run:
punched_cards.exe < punched_cards.in

Input:

3
3 4
2 2
2 3

Output:

Case #1: 
..+-+-+-+
..|.|.|.|
+-+-+-+-+
|.|.|.|.|
+-+-+-+-+
|.|.|.|.|
+-+-+-+-+

Case #2:
..+-+
..|.|
+-+-+
|.|.|
+-+-+

Case #3:
..+-+-+
..|.|.|
+-+-+-+
|.|.|.|
+-+-+-+

*/

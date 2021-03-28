
#include <algorithm>
#include <iostream>
#include <string>


// CJ cost X
// JC cost Y
static void smart() // inspired by ecnerwala
{
    int X, Y; std::string text; std::cin >> X >> Y >> text;
    const int INF = 1e9, size = (int)text.size();

    int pos{0}, costIfNextC{0}, costIfNextJ{0};
    while (pos < size) {
        if (pos > 0) {
            const int oldCostIfNextC = costIfNextC;
            costIfNextC = std::min(costIfNextJ + Y, costIfNextC);
            costIfNextJ = std::min(oldCostIfNextC + X, costIfNextJ);
        }
        const char cur = text[pos++];
        if (cur == 'J') costIfNextC = INF;
        else if (cur == 'C') costIfNextJ = INF;
    }

    std::cout << std::min(costIfNextC, costIfNextJ);
}

static void solve()
{
    int X, Y; std::string text; std::cin >> X >> Y >> text;
    const int size = (int)text.size();

    char prev = '-';
    int pos{0}, cost{0};
    while (pos < size) {
        while (pos < size && (text[pos] == prev || text[pos] == '?')) ++pos;
        if (pos == size) break;
        const char cur = text[pos];
        if (prev == 'C' && cur == 'J') cost += X;
        if (prev == 'J' && cur == 'C') cost += Y;
        prev = cur;
    }

    std::cout << cost;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; smart(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 moons_and_umbrellas.cpp -o moons_and_umbrellas.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address moons_and_umbrellas.cpp -o moons_and_umbrellas.o

Run:
moons_and_umbrellas.exe < moons_and_umbrellas.in

Input:

5
2 3 CJ?CC?
4 2 CJCJ
1 3 C?J
2 5 ??J???
5 7 ?????????CCC?JJ????C?????

Output:

Case #1: 5
Case #2: 10
Case #3: 1
Case #4: 0
Case #5: 12

*/

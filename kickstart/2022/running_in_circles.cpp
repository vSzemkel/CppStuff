
#include <iostream>

// Running in Circles
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb1b6/0000000000c4766e

static void solve() {
    char dirChar;
    int L, N, delta;
    std::cin >> L >> N >> delta >> dirChar;

    int pos = delta % L;
    bool dir = dirChar == 'C';
    int64_t score = delta / L;

    for (int z = N - 1; z; --z) {
        std::cin >> delta >> dirChar;
        if (dir == (dirChar == 'C')) {
            const int cur = pos + delta;
            score += cur / L;
            pos = cur % L;
        } else {
            const int cur = pos - delta;
            if (cur > 0)
                pos = cur;
            else {
                pos = -cur % L;
                score += -cur / L;
                dir = !dir;
            }
        }
    }

    std::cout << score;
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 running_in_circles.cpp -o running_in_circles.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address running_in_circles.cpp -o running_in_circles

Run:
running_in_circles.exe < running_in_circles.in

Input:

3
5 3
8 C
4 A
5 C
4 5
2 C
8 A
3 A
5 C
8 A
4 3
3 C
2 A
5 C

Output:

Case #1: 1
Case #2: 5
Case #3: 1

*/

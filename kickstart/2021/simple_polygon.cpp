
#include <assert.h>
#include <iostream>

// Simple polygon
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000004362d6/00000000008b36f9

static void solve() {
    int N, A; std::cin >> N >> A;
    assert(N > 2);

    if (N == 3)
        std::cout << "POSSIBLE\n0 0\n" << A << " 0\n0 1\n";
    else {
        const int skip = A + 2 - N;
        if (skip < 0)
            std::cout << "IMPOSSIBLE";
        else {
            const int jump = (skip / 2) + 1;
            int loops = (A - 2 * jump) / 2;
            int x{jump}, y{jump}, delta{-1};
            std::cout << "POSSIBLE\n0 0\n";
            if (A & 1) {
                if (N & 1) {
                    std::cout << x << ' ' << y << '\n';
                    for (int z = loops; z; --z) {
                        x += delta;
                        std::cout << x << ' ' << ++y << '\n';
                        delta = -delta;
                    }

                    if (delta < 0) {
                        std::cout << x << ' ' << ++y << '\n';
                        std::cout << ++x << ' ' << --y << '\n';
                    } else {
                        std::cout << ++x << ' ' << ++y << '\n';
                        std::cout << x << ' ' << --y << '\n';
                    }

                    for (int z = loops; z; --z) {
                        x += delta;
                        std::cout << x << ' ' << --y << '\n';
                        delta = -delta;
                    }
                } else {
                    std::cout << ++x << ' ' << ++y << '\n';
                    for (int z = loops; z; --z) {
                        y += delta;
                        std::cout << ++x << ' ' << y << '\n';
                        delta = -delta;
                    }

                    std::cout << x << ' ' << --y << '\n';

                    for (int z = loops; z; --z) {
                        y += delta;
                        std::cout << --x << ' ' << y << '\n';
                        delta = -delta;
                    }
                }
            } else {
                std::cout << x << ' ' << y << '\n';
                if (N & 1) {
                    --loops;
                    for (int z = loops; z; --z) {
                        x += delta;
                        std::cout << x << ' ' << ++y << '\n';
                        delta = -delta;
                    }

                    y += 2;
                    std::cout << x << ' ' << y << '\n';
                    y -= 2;
                    std::cout << ++x << ' ' << y << '\n';
                } else {
                    for (int z = loops; z; --z) {
                        x += delta;
                        std::cout << x << ' ' << ++y << '\n';
                        delta = -delta;
                    }

                    std::cout << ++x << ' ' << y << '\n';
                }

                for (int z = loops; z; --z) {
                    x += delta;
                    std::cout << x << ' ' << --y << '\n';
                    delta = -delta;
                }
            }

            std::cout << "1 0\n";
        }
    }
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 simple_polygon.cpp -o simple_polygon.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address simple_polygon.cpp -o simple_polygon

Run:
simple_polygon.exe < simple_polygon.in

Input:

4
7 5
4 5
5 2
4 36

Output:

Case #1: POSSIBLE
0 0
1 1
2 2
3 3
3 2
2 1
1 0

Case #2: POSSIBLE
0 0
3 3
3 2
1 0

Case #3: IMPOSSIBLE
Case #4: POSSIBLE
0 0
18 18
19 18
1 0

*/

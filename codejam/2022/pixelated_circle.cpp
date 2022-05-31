
#include <iostream>

// Pixelated Circle
// https://codingcompetitions.withgoogle.com/codejam/round/00000000008778ec/0000000000b158f7

// correct: draw everything such that x * x + y * y <= R * R + R + 0.5
//
// incorrect: draw such that x * x + y * y - y + 0.25 <= r * r <= x * x + y * y + y + 0.25 for integer r
// we know that x * x + y * y < r * r + y <= R * R + R, so this is a subset of the full circle
// We just want to know the number of points so that x * x + y * y - max(x, y) + 1 .. x * x + y * y + max(x, y) does not include a perfect square
// Each point has a unique radius, so we can just count.

static void solve() { // ecnerwala
    int64_t R;
    std::cin >> R;

    int64_t ans{0};
    for (int64_t x = 1, y = R; x <= R; ++x) {
        while (x * x + y * y > R * R + R)
            y--;
        ans += y;
    }
    for (int64_t r = 1, x = 0; r <= R; ++r) {
        while ((x + 1) * (x + 1) + (x + 2) * (x + 2) - (x + 2) < r * r)
            x++;

        int64_t cnt = x * 2;
        if ((x + 1) * (x + 1) + (x + 1) * (x + 1) - (x + 1) < r * r)
            cnt++;

        ans -= cnt;
    }

    std::cout << ans * 4;
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 pixelated_circle.cpp -o pixelated_circle.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address pixelated_circle.cpp -o pixelated_circle

Run:
pixelated_circle.exe < pixelated_circle.in

Input:

3
2
8
50

Output:

Case #1: 4
Case #2: 24
Case #3: 812

*/

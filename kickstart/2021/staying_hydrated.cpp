
#include <algorithm>
#include <iostream>
#include <vector>

// Staying Hydrated
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000004362d6/00000000008b3a1c

const auto INF = std::numeric_limits<int>::max() / 2;

static void solve_set1() { // MLE
    int K; std::cin >> K;
    int xmin{INF}, ymin{INF}, xmax{-INF}, ymax{-INF};
    std::vector<int> xl(K), xr(K), yb(K), yt(K); // l <= r; b <= t
    for (int i = 0; i < K; ++i) {
        std::cin >> xl[i] >> yb[i] >> xr[i] >> yt[i];
        xmin = std::min(xmin, xl[i]);
        xmax = std::max(xmax, xr[i]);
        ymin = std::min(ymin, yb[i]);
        ymax = std::max(ymax, yt[i]);
    }

    const int width = xmax - xmin + 1;
    const int heigth = ymax - ymin + 1;

    std::vector<int> hor(width), ver(heigth);
    for (int i = 0; i < K; ++i) {
        for (int x = xl[i] - xmin - 1; x >= 0; --x)
            hor[x] += xl[i] - xmin - x;
        for (int x = xr[i] - xmin + 1; x < width; ++x)
            hor[x] += x - xr[i] + xmin;
        for (int y = yb[i] - ymin - 1; y >= 0; --y)
            ver[y] += yb[i] - ymin - y;
        for (int y = yt[i] - ymin + 1; y < heigth; ++y)
            ver[y] += y - yt[i] + ymin;
    }

    const int optx = *std::min_element(hor.begin(), hor.end());
    int retx{0}; while (hor[retx] != optx) ++retx;
    const int opty = *std::min_element(ver.begin(), ver.end());
    int rety{0}; while (ver[rety] != opty) ++rety;

    std::cout << xmin + retx << ' ' << ymin + rety;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve_set1(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 staying_hydrated.cpp -o staying_hydrated.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address staying_hydrated.cpp -o staying_hydrated

Run:
staying_hydrated.exe < staying_hydrated.in

Input:

2
3
0 0 1 1
2 3 4 6
0 3 5 9
1
0 0 1 1

Output:

Case #1: 1 3
Case #2: 0 0

*/

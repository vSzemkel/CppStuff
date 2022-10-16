
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

// Curling
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb2e1/0000000000c17c82

static void solve() {
    int64_t RS, RH;
    std::cin >> RS >> RH;
    const auto bound = (RS + RH) * (RS + RH);

    int N; std::cin >> N;
    std::vector<std::pair<int64_t, bool>> scores(N); // {distance, team}
    for (int i = 0; i < N; ++i) {
        int64_t x, y;
        std::cin >> x >> y;
        scores[i] = {x * x + y * y, false};
    }
    int M; std::cin >> M;
    scores.resize(N + M);
    for (int i = N; i < N + M; ++i) {
        int64_t x, y;
        std::cin >> x >> y;
        scores[i] = {x * x + y * y, true};
    }

    std::sort(scores.begin(), scores.end());

    int red{0};
    for (const auto s : scores) {
        if (s.first > bound)
            break;
        if (s.second == true)
            break;
        ++red;
    }

    int yellow{0};
    for (const auto s : scores) {
        if (s.first > bound)
            break;
        if (s.second == false)
            break;
        ++yellow;
    }

    std::cout << red << ' ' << yellow;
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 curling.cpp -o curling.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address curling.cpp -o curling

Run:
curling.exe < curling.in

Input:

2
1 5
2
1 0
-3 0
1
0 2
10 50
2
-40 -31
-35 70
3
59 0
-10 0
30 40

Output:

Case #1: 1 0
Case #2: 0 2

*/

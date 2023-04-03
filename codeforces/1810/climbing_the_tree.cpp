
#include <algorithm>
#include <cassert>
#include <iostream>

// Climbing The Tree
// https://codeforces.com/contest/1810/problem/D

static void solve() {
    int Q; std::cin >> Q;
    int64_t A, B, N, hmin{0}, hmax = 8e18;
    for (int i = 0; i < Q; ++i) {
        int ev; std::cin >> ev >> A >> B;
        const auto progress = A - B;
        if (ev == 1) {
            std::cin >> N;
            const auto low = (N == 1 ? 0 : progress * (N - 2) + A) + 1;
            const auto high = (N - 1) * progress + A;
            if (hmax < low || high < hmin)
                std::cout << "0 ";
            else {
                std::cout << "1 ";
                hmin = std::max(hmin, low);
                hmax = std::min(hmax, high);
            }
        } else {
            const auto get_time = [&](int64_t h) -> int64_t {
                if (h <= A) return 1;
                return (h - A + progress - 1) / progress + 1;
            };
            const auto low  = get_time(hmin);
            const auto high = get_time(hmax);

            if (low < high)
                std::cout << "-1 ";
            else
                std::cout << low << ' ';
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
        solve(); std::cout << '\n';
    }
}

/*

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++17 climbing_the_tree.cpp -o climbing_the_tree.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address climbing_the_tree.cpp -o climbing_the_tree

Run:
climbing_the_tree.exe < climbing_the_tree.in

Input:

5
3
1 3 2 5
2 4 1
2 3 2
3
1 6 5 1
2 3 1
2 6 2
3
1 4 2 2
1 2 1 3
2 10 2
9
1 7 3 6
1 2 1 8
2 5 1
1 10 9 7
1 8 1 2
1 10 5 8
1 10 7 7
2 7 4
1 9 4 2
9
1 2 1 6
1 8 5 6
1 4 2 7
2 9 1
1 5 1 4
1 5 2 7
1 7 1 9
1 9 1 4
2 10 8

Output:

1 2 5
1 -1 1
1 0 1
1 0 -1 0 0 0 1 8 0 
1 0 0 1 0 0 0 0 1 

*/

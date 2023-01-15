
#include <algorithm>
#include <cmath>
#include <iostream>
#include <unordered_set>
#include <vector>

// Street Checkers
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050edb/00000000001707b9

std::unordered_set<int> good, bad;

static void solve() {
    int L, R;
    std::cin >> L >> R;

    int ans{0};
    std::vector<bool> painted;
    for (int x = L; x <= R; ++x) {
        if (good.count(x)) {
            ++ans;
            continue;
        }
        if (bad.count(x))
            continue;

        painted.clear();
        int c{1};
        for (; c * c < x; ++c) {
            const auto d = x / c;
            if (c * d == x) {
                painted.push_back(c % 2);
                painted.push_back(d % 2);
            }
        }
        if (c * c == x)
            painted.push_back(c % 2);

        const auto popcnt = std::count_if(painted.begin(), painted.end(), [](const auto b){ return b; });
        if (std::abs(int(painted.size() - 2 * popcnt)) < 3) {
            good.insert(x);
            ++ans;
        } else
            bad.insert(x);
    }

    std::cout << ans;
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 street_checkers.cpp -o street_checkers.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address street_checkers.cpp -o street_checkers

Run:
street_checkers.exe < street_checkers.in

Input:

2
5 10
102 102
1974 2075

Output:

Case #1: 5
Case #2: 1
Case #3: 43

*/

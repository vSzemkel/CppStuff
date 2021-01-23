
#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <utility>
#include <vector>

// Wormhole In One
// https://codingcompetitions.withgoogle.com/codejam/round/000000000019ffb9/00000000003386d0


int solve() {
    int N; std::cin >> N;
    std::vector<std::pair<int, int>> points(N);
    for (int i = 0; i < N; ++i)
        std::cin >> points[i].first >> points[i].second;

    std::map<std::pair<int, int>, std::set<std::pair<int, int>>> angles; // angle -> set of points
    for (int i = 0; i < N - 1; ++i)
        for (int j = i + 1; j < N; ++j) {
            const auto& pi = points[i];
            const auto& pj = points[j];
            int dx = pj.first - pi.first;
            int dy = pj.second - pi.second;
            if (dx < 0) { // normalize
                dx *= -1;
                dy *= -1;
            }
            if (dx == 0) // special
                dy = 1;
            else if (dy == 0)
                dx = 1;
            else { // common
                const auto gd = std::gcd(dx, dy);
                dx /= gd; dy /= gd;
            }
            auto& a = angles[{dx, dy}];
            a.insert(pi);
            a.insert(pj);
        }

    const auto it = std::max_element(angles.begin(), angles.end(), [](const auto& a1, const auto& a2){ return a1.second.size() < a2.second.size(); });
    int ret = it == angles.end() ? 0 : it->second.size();
    if (ret < N) ++ret;
    if (ret < N && (ret % 2) == 1) ++ret;

    return ret;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": " << solve() << "\n";
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 wormhole_in_one.cpp -o wormhole_in_one.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 wormhole_in_one.cpp -o wormhole_in_one.o

Run:
wormhole_in_one.exe < wormhole_in_one.in

Input:

5
2
0 0
5 5
3
0 0
5 5
5 0
5
0 0
5 5
5 0
3 2
2 4
7
0 0
1 1
2 1
3 1
8 2
11 2
14 2
1
-1000000000 1000000000

Output:

Case #1: 2
Case #2: 3
Case #3: 4
Case #4: 7
Case #5: 1

*/

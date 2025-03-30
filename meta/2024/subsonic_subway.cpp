
#include <algorithm>
#include <cassert>
#include <format>
#include <iomanip>
#include <iostream>
#include <vector>

// Subsonic Subway
// https://www.facebook.com/codingcompetitions/hacker-cup/2024/round-1/problems/A

static const double FAST = std::numeric_limits<double>::max();

static void solve()
{
    int N;
    std::cin >> N;
    std::vector<double> vlo(N), vhi(N);
    for (int i = 0; i < N; ++i) {
        int tlo, thi;
        std::cin >> tlo >> thi;
        vlo[i] = double(i + 1) / thi;
        vhi[i] = tlo == 0 ? FAST : double(i + 1) / tlo;
        assert(vlo[i] < vhi[i]);
    }

    const auto min_vlo = *std::max_element(vlo.begin(), vlo.end());
    const auto max_vhi = *std::min_element(vhi.begin(), vhi.end());
    std::cout << (max_vhi < min_vlo ? -1.0 : min_vlo);
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << std::format("Case #{}: ", g) << std::fixed << std::setprecision(9);
        solve();
        std::cout << '\n';
    }
}

/*

Format:
clang-format -i subsonic_subway.cpp

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 subsonic_subway.cpp -o subsonic_subway.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address subsonic_subway.cpp -o subsonic_subway

Run:
subsonic_subway.exe < subsonic_subway.in

Input:

4
3
0 10
0 10
0 10
4
0 3
1 4
1 2
0 10
2
5 10
0 5
2
1 2
4 10

Output:

Case #1: 0.300000000
Case #2: 1.500000000
Case #3: -1.000000000
Case #4: 0.500000000

*/

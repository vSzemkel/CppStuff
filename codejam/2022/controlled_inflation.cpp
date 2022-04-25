
#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <vector>

// Controlled Inflation
// https://codingcompetitions.withgoogle.com/codejam/round/000000000087711b/0000000000accfdb

constexpr const int64_t INF = 1e18;

std::vector<std::array<int, 2>> mM; // minmax for customers
std::vector<std::vector<int>> baloons; // requirements
std::vector<std::vector<std::map<std::pair<int, int>, int64_t>>> cache;

/**
 * @brief One of optimal processes must end on extremal pump setting
 * 
 * @param first first customer
 * @param last last customer (inclusive)
 * @param initval init pump value
 * @param finval final pump value
 * @return int64_t minimal setting change
 */
static int64_t solve_inner(const int first, const int last, int initval, int finval) {
    if (first == last) {
        if (finval < initval)
            std::swap(finval, initval);
        return finval - initval + 2 * (std::max(initval - mM[first][0], 0) + std::max(mM[first][1] - finval, 0));
    }

    // cache usage is mandatory to avoid TLE
    const auto found = cache[first][last].find({initval, finval});
    if (found != cache[first][last].end())
        return found->second;

    int64_t ans{INF};
    const int mid = first + (last - first) / 2;
    for (int i : {0, 1})
        ans = std::min(ans, solve_inner(first, mid, initval, mM[mid][i]) + solve_inner(mid + 1, last, mM[mid][i], finval));

    return cache[first][last][{initval, finval}] = ans;
}

static void solve() {
    int N, P;
    std::cin >> N >> P;
    baloons.assign(N, std::vector<int>(P));
    cache.assign(N, std::vector<std::map<std::pair<int, int>, int64_t>>(N));
    for (auto& r : baloons)
        for (auto& b : r)
            std::cin >> b;
    mM.resize(N);
    for (int i = 0; i < N; ++i) {
        const auto [m, M] = std::minmax_element(baloons[i].begin(), baloons[i].end());
        mM[i][0] = *m; mM[i][1] = *M;
    }

    int64_t ans{INF};
    for (int b = 0; b < P; ++b)
        for (int i : {0, 1})
            ans = std::min(ans, solve_inner(0, N - 1, 0, mM[N - 1][i]));

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
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 controlled_inflation.cpp -o controlled_inflation.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address controlled_inflation.cpp -o controlled_inflation

Run:
controlled_inflation.exe < controlled_inflation.in

Input:

2
3 3
30 10 40
20 50 60
60 60 50
5 2
1 1000000000
500000000 1000000000
1 1000000000
500000000 1
1 1000000000

Output:

Case #1: 110
Case #2: 4999999996

*/


#include <algorithm>
#include <array>
#include <assert.h>
#include <bitset>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <filesystem>
#include <fstream>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <stdlib.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <vector>

// Twisty Little Passages
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000876ff1/0000000000a45fc0

std::ofstream g_debug("twisty_little_passages.log");

const auto rand_in_range = [](const int64_t ubound){std::random_device seed;std::mt19937 gen{seed()};std::uniform_int_distribution<int64_t> dist(0,ubound-1);return dist(gen);};

/**
 * @brief Nodes visited randomle with T are statisticaly correct for giving an avarage for unknown nodes
 * Nodes visited with W are not a uniform sample as the graph may not be uniform
 * Use information about every known node and use it only once
 */
static void solve() {
    int N, K, R, P;
    std::cin >> N >> K >> R >> P; --R;

    int q{0}, countedT{1}, countedW{0};
    std::vector<bool> visited(N);
    int64_t totalW{0}, totalT{P};
    visited[R] = true;

    while (true) {
        std::cout << 'W' << std::endl;
        std::cin >> R >> P; --R;
        if (!visited[R]) {
            visited[R] = true;
            totalW += P;
            ++countedW;
        }
        if (++q == K) break;

        std::cout << "T " << rand_in_range(N) + 1 << std::endl;
        std::cin >> R >> P; --R;
        if (!visited[R]) {
            visited[R] = true;
            totalT += P;
            ++countedT;
        }
        if (++q == K) break;
    }

    const double unknwn = N - countedW - countedT;
    const auto ans = totalT * unknwn / countedT;
    std::cout << "E " << int64_t(nearbyint((ans + totalW + totalT) / 2)) << std::endl;
}

/**
 * @brief This forks pretty well for unifomly densed graphs
 */
static void naive() { // WA
    int N, K, R, P;
    std::cin >> N >> K;
    const int q = std::min(N, K);
    g_debug << "Next case: " << N << ' ' << K << std::endl;

    int counted{0};
    int64_t total{0};
    std::vector<bool> visited(N);
    for (int z = q; z; --z) {
        std::cin >> R >> P; --R;
        g_debug << "Next move [R, P, Z, T]: " << R << ' ' << P << ' ' << z << ' ' << total << std::endl;
        if (!visited[R]) {
            total += P;
            ++counted;
            visited[R] = true;
        }

        if (P > 5 && 2 * counted < N)
            std::cout << 'W' << std::endl;
        else {
            if (z > 1)
                while (visited[R]) R = (R + 1) % N;
            std::cout << "T " << R + 1 << std::endl;
        }
    }

    std::cin >> R >> P; // just eat
    const auto ans = total * N / counted / 2;
    std::cout << "E " << ans << std::endl;
    g_debug << "E " << ans << std::endl;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g)
        solve();
}

/*

Compile:
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 twisty_little_passages.cpp -o twisty_little_passages.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address twisty_little_passages.cpp -o twisty_little_passages

Run:
py.exe interactive_runner.py py.exe twisty_little_passages_testing_tool.py 0 -- twisty_little_passages.exe

*/

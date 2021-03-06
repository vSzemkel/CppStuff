
#include <iostream>
#include <numeric>
#include <vector>

// Thermometers
// https://codingcompetitions.withgoogle.com/codejam/round/000000000019ff7e/000000000037776b#analysis
// This solution is inspiered by ecnerwala

/*

X1 - d1 = d2
X2 - d2 = d3
...
XN - dn = d1


d1 = X1 - d2
d1 = X1 - X2 + d3
d1 = X1 - X2 + X3 - d4

if (N % 2 == 0)
    0 = X1 - X2 + .. XN-1 - XN
else
    2d1 = X1 - X2 + .. XN-2 - XN-1 + XN

*/

int solve() {
    int64_t K; int N; std::cin >> K >> N;
    std::vector<int64_t> X(N + 1);
    for (int i = 0; i < N; ++i)
        std::cin >> X[i];
    int ignore;
    for (int i = 0; i < N; ++i)
        std::cin >> ignore; // temperatures doesn't matter

    // normalize input
    for (int i = 1; i < N; ++i)
        X[i] -= X[0];
    X[0] = 0;
    X[N] = K;

    // try to construct optimal solution: N
    int sign{1};
    int64_t dinit = K - X[N - 1];
    int64_t sum{0}, lo{0}, hi{dinit}, dprev{dinit};
    for (int i = 0; i < N; ++i) {
        const auto dcurr = X[i + 1] - X[i];
        const auto old_lo = lo;
        sum += sign * dcurr;
        sign *= -1;
        lo = std::min(dcurr, dprev - hi);
        hi = std::min(dcurr, dprev - old_lo);
        dprev = dcurr;
    }

    // close the circle
    const auto old_lo = lo;
    const auto dcurr = X[1];
    lo = std::min(dcurr, dprev - hi);
    hi = std::min(dcurr, dprev - old_lo);

    if (N % 2 == 0) {
        if (sum == 0 && lo < hi) return N;
    } else
        if (2 * lo < sum && sum < 2 * hi) return N;

    // need more then N thermomenters
    X.resize(2 * N + 1);
    for (int i = 1; i <= N; ++i)
        X[N + i] = K + X[i];

    std::vector<int> transfer(N);
    for (int i = 0; i < N; ++i) {
        lo = 0; hi = X[N + i] - X[N + i - 1];
        auto dprev = hi;
        int j = i;
        while (j < i + N) {
            const auto dcurr = X[j + 1] - X[j];
            const auto old_lo = lo;
            lo = std::min(dcurr, dprev - hi);
            hi = std::min(dcurr, dprev - old_lo);
            if (lo >= hi)
                break;
            dprev = dcurr;
            ++j;
        }
        transfer[i] = j - i;
    }

    int ret{2 * N};
    for (int i = 0; i < N; ++i) {
        int len{0};
        int thermo{N};
        while (len < N) {
            ++thermo;
            len += transfer[(i + len) % N];
        }
        ret = std::min(ret, thermo);
    }

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
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 thermometers.cpp -o thermometers.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 thermometers.cpp -o thermometers.o

Run:
thermometers.exe < thermometers.in

Input:

5
2 2
0 1
184 330
3 2
0 1
184 330
10 3
1 5 9
184 200 330
4 3
0 1 2
333 334 335
4 3
0 2 3
333 334 335

Output:

Case #1: 2
Case #2: 3
Case #3: 3
Case #4: 4
Case #5: 4

*/

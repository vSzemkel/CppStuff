
#include <iostream>
#include <numeric>
#include <vector>

// Thermometers
// https://codingcompetitions.withgoogle.com/codejam/round/000000000019ff7e/000000000037776b#analysis

// THIS IS DIFFICULT TASK FOR ME AND THESE ARE MORE NOTES AND PoC THEN SOLUTION


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


lo = max(lo, X - hi)
hi = min(hi, X - lo)

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
    int64_t sum{0}, lo{0}, hi{dinit};
    for (int i = 0; i < N; ++i) {
        const auto dprev = i == 0 ? hi : X[i] - X[i - 1];
        const auto dcurr = X[i + 1] - X[i];
        sum += sign * dcurr;
        sign *= -1;
        lo = std::max(lo, dprev - hi);
        lo = std::min(lo, dcurr);
        hi = std::min(hi, dprev - lo);
        hi = std::min(hi, dcurr);
    }

    if (N % 2 == 0) {
        if (sum == 0 && lo <= hi) return N;
    } else
        if (0 <= sum && sum <= 2 * dinit) return N;

    // need more then N thermomenters
    X.resize(2 * N + 1);
    for (int i = 1; i < N; ++i)
        X[N + i] = K + X[i];

    std::vector<int> transfer(N, 1);
    for (int i = 0; i < N; ++i) {
        lo = 0; hi = X[i + 1] - X[i];
        for (int j = i; j < i + N; ++j) {
            const auto dprev = j == 0 ? hi : X[j] - X[j - 1];
            const auto dcurr = X[j + 1] - X[j];
            lo = std::max(lo, dprev - hi);
            lo = std::min(lo, dcurr);
            hi = std::min(hi, dprev - lo);
            hi = std::min(hi, dcurr);
            if (lo >= hi) {
                transfer[i] = j - i + 1;
                break;
            }
        }
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
py.exe interactive_runner.py py.exe thermometers_testing_tool.py 1 -- thermometers.exe
thermometers.exe < thermometers.in

Input:

3
2 2
0 1
184 330
3 2
0 1
184 330
10 3
1 5 9
184 200 330

Output:


*/


#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

// Subtransmutation
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000435baf/00000000007ae4aa

constexpr int MAXAMOUNT = 100000;

int N, A, B;
std::vector<int> quantities;

static bool feasible() {
    // every produced item can be presented as: M - k*A - n*B == M - gcd(A, B) * X
    // M - Item == gcd(A, B) *X
    // Item1 - Item2 % gcd == 0
    const int g = std::gcd(A, B);
    for (int i = 0; i < N - 1; ++i)
        for (int j = i + 1; j < N; ++j)
            if (quantities[i] > 0 && quantities[j] > 0 && (i - j) % g != 0)
                return false;
    return true;
}

static bool check(int n) {
    std::vector<int> assets(n + 1);
    assets[n] = 1;
    while (n >= 0) {
        int excess = assets[n];
        if (n < N) {
            if (quantities[n] > assets[n])
                return false;
            excess -= quantities[n];
        }
        if (n - A >= 0) assets[n - A] = std::min(assets[n - A] + excess, MAXAMOUNT);
        if (n - B >= 0) assets[n - B] = std::min(assets[n - B] + excess, MAXAMOUNT);
        --n;
    }

    return true;
}

static void solve() {
    std::cin >> N >> A >> B;
    quantities.resize(N);
    for (int i = 0; i < N; ++i)
        std::cin >> quantities[i];

    if (feasible()) {
        int ret{N - 1};
        while (true) {
            if (check(ret)) {
                std::cout << ret + 1;
                return;
            }
            ++ret;
        }
    }

    std::cout << "IMPOSSIBLE";
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 subtransmutation.cpp -o subtransmutation.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address subtransmutation.cpp -o subtransmutation.o

Run:
subtransmutation.exe < subtransmutation.in

Input:

6
2 1 2
1 2
5 1 2
2 0 0 0 1
3 1 2
1 1 1
3 2 4
1 1 1
3 2 4
1 0 1
5 2 5
1 0 0 0 1

Output:

Case #1: 4
Case #2: 6
Case #3: 5
Case #4: IMPOSSIBLE
Case #5: 5
Case #6: 10

*/

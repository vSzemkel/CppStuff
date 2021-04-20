
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

// Wandering Robot
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ffc8/00000000002d8565

constexpr int MAX = 200020;
double logFactorials[MAX];

double choose(const int n, const int k) {
    if (k < 0 || k > n) return 0;
    if (k == 0 && n == 0) return 1;
    return std::pow(2, logFactorials[n] - logFactorials[k] - logFactorials[n - k] - n);
}

static void solve() {
    int64_t CC, RR, L, T, R, B; std::cin >> CC >> RR >> L >> T >> R >> B;
    --L; --T; --R; --B;

    double ret{0};
    if (R < CC - 1) {
        int r = T;
        int c = R;
        double factor{1};
        while (r > 0) {
            --r; ++c;
            if (c == CC - 1) {
                --c;
                factor = 0.5;
            }
            ret += factor * choose(r + c, r);
        }
    }

    if (B < RR - 1) {
        int r = B;
        int c = L;
        double factor{1};
        while (c > 0) {
            ++r, --c;
            if (r == RR - 1) {
                --r;
                factor = 0.5;
            }
            ret += factor * choose(r + c, r);
        }
    }

    std::cout << std::fixed << std::setprecision(6) << ret;
}

static void solve_complement() { // inspired by ecnerwala
    int64_t CC, RR, L, T, R, B; std::cin >> CC >> RR >> L >> T >> R >> B;
    --L; --T; --R; --B;

    double ret{0};
    if (T > 0) {
        for (int64_t x = L; x <= R; ++x)
            ret += 0.5 * choose(x + T - 1, x);
        if (R == CC - 1)
            for (int64_t y = 0; y < T; ++y)
                ret += 0.5 * choose(y + CC - 1, y);
    }

    std::swap(RR, CC);
    std::swap(L, T);
    std::swap(R, B);
    if (T > 0) {
        for (int64_t x = L; x <= R; ++x)
            ret += 0.5 * choose(x + T - 1, x);
        if (R == CC - 1)
            for (int64_t y = 0; y < T; ++y)
                ret += 0.5 * choose(y + CC - 1, y);
    }

    std::cout << std::fixed << std::setprecision(6) << (1 - ret);
}

static void solve_swap() { // inspired by scottwu
    int64_t CC, RR, L, T, R, B; std::cin >> CC >> RR >> L >> T >> R >> B;
    --L; --T; --R; --B;

    double ret{0};
    if (R < CC - 1) {
        int r = T;
        int c = R;
        double factor{1};
        while (r > 0) {
            --r; ++c;
            if (c == CC - 1) {
                --c;
                factor = 0.5;
            }
            ret += factor * choose(r + c, r);
        }
    }

    std::swap(RR, CC);
    std::swap(L, T);
    std::swap(R, B);
    if (R < CC - 1) {
        int r = T;
        int c = R;
        double factor{1};
        while (r > 0) {
            --r; ++c;
            if (c == CC - 1) {
                --c;
                factor = 0.5;
            }
            ret += factor * choose(r + c, r);
        }
    }

    std::cout << std::fixed << std::setprecision(6) << ret;
}

static void solve_small() {
    const auto bin_coeff = [](const int n, const int k){return 1/((n+1)*std::beta(n-k+1,k+1));};
    int64_t CC, RR, L, T, R, B; std::cin >> CC >> RR >> L >> T >> R >> B;
    --L; --T; --R; --B;

    double ret{0};
    int r = T - 1;
    int c = R + 1;
    while (r >= 0 && c < CC) {
        ret += bin_coeff(r + c, r) / (1 << (r + c));
        --r; ++c;
    }

    r = B + 1;
    c = L - 1;
    while (c >= 0 && r < RR) {
        ret += bin_coeff(r + c, r) / (1 << (r + c));
        ++r, --c;
    }

    std::cout << std::setprecision(10) << ret;
}

static void dp() { // for big RR*CC board does not fit in memory
    int64_t CC, RR, L, T, R, B; std::cin >> CC >> RR >> L >> T >> R >> B;

    --L; --T; --R; --B;
    CC = std::min(CC, R + 2);
    RR = std::min(RR, B + 2);

    std::vector<std::vector<double>> board(RR, std::vector<double>(CC, -1));
    for (int r = T; r <= B; ++r)
        for (int c = L; c <= R; ++c)
            board[r][c] = 0;
    int mark{1};
    for (int r = RR - 1; r >= 0; --r) {
        auto& cell = board[r][CC - 1];
        if (cell == 0)
            mark = 0;
        cell = mark;
    }
    mark = 1;
    for (int c = CC - 1; c >= 0; --c) {
        auto& cell = board[RR - 1][c];
        if (cell == 0)
            mark = 0;
        cell = mark;
    }

    for (int r = RR - 2; r >= 0; --r)
        for (int c = CC - 2; c >= 0; --c) {
            auto& cell = board[r][c];
            if (cell != 0)
                cell = 0.5 * (board[r + 1][c] + board[r][c + 1]);
        }

    std::cout << board[0][0];
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    logFactorials[0] = 0.0;
    for (int i = 1; i < MAX; i++)
        logFactorials[i] = logFactorials[i-1] + std::log2(i);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 wandering_robot.cpp -o wandering_robot.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address wandering_robot.cpp -o wandering_robot.o

Run:
wandering_robot.exe < wandering_robot.in

Input:

4
3 3 2 2 2 2
5 3 1 2 4 2
1 10 1 3 1 5
6 4 1 3 3 4

Output:

Case #1: 0.5
Case #2: 0.0625
Case #3: 0
Case #4: 0.3125

*/

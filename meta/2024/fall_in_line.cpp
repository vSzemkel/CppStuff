
#include <iostream>
#include <random>
#include <vector>

// Fall in line
// https://www.facebook.com/codingcompetitions/hacker-cup/2024/practice-round/problems/C

template <typename T = int>
static T rand_in_range(const T ubound) {
    static std::random_device seed;
    static std::mt19937 gen{seed()};
    std::uniform_int_distribution<T> dist(0, ubound - 1);
    return dist(gen);
};

template <typename T = int>
struct point_t {
    point_t(T _x = T{}, T _y = T{}) : x(_x), y(_y) {}
    T cross(const point_t& p) const { return x * p.y - y * p.x; }
    T cross(const point_t& a, const point_t& b) const { return (a - *this).cross(b - *this); }
    point_t operator-(const point_t& p) const { return point_t{x - p.x, y - p.y}; }

    T x, y;
};

/**
 * Let's P denotes maximal numer of in line points
 * Expected output M is a number between (N - P) and (N - P) * 2
 * We need to find a line with between N - (N - P) * 2 and P points
 * We need to find a line with B points on it, when B is between N - 2P and P points
 * The answer is N - B
 */
static constexpr int RUNS = 100;

static void solve() {
    int N;
    std::cin >> N;
    std::vector<point_t<>> points(N);
    for (auto& p : points)
        std::cin >> p.x >> p.y;

    int B{};
    for (int z = RUNS; z; --z) {
        const int i = rand_in_range(N);
        const int j = rand_in_range(N);
        if (i == j) {
            ++z;
            continue;
        }

        const auto& p1 = points[i];
        const auto& p2 = points[j];
        int b{2};
        for (int k = 0; k < N; ++k) {
            if (k == i || k == j)
                continue;
            if (points[k].cross(p1, p2) == 0)
                ++b;
        }
        if (B < b)
            B = b;
        if (2 * B > N)
            break;
    }

    std::cout << N - B;
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

Format:
clang-format -i fall_in_line.cpp

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 fall_in_line.cpp -o fall_in_line.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address fall_in_line.cpp -o fall_in_line

Run:
fall_in_line.exe < fall_in_line.in

Input:

3
4
1 1
2 2
-3 -3
4 4
4
1 1
-1 1
1 -1
-1 -1
7
4 8
2 4
7 2
6 10
0 1
3 4
4 7

Output:

Case #1: 0
Case #2: 2
Case #3: 3

*/

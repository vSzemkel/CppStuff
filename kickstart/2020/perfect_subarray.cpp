
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

// Perfect Subarray
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ff43/00000000003381cb

std::vector<int> g_input, g_partial, g_squares;

constexpr int MAXSUM = 10'000'000;

const auto range_sum = [](const int i, const int j){return g_partial[j + 1] - g_partial[i];};

static void solve() { // adapted from Errichto
    int N; std::cin >> N;
    g_input.resize(N);
    int abssum{0}, prefsum{0};
    for (auto& n : g_input) {
        std::cin >> n;
        abssum += std::abs(n);
    }

    // cnt[abssum + X] is the number of prefixes g_input[0..i] giving the sum X
    std::vector<int> cnt(2 * abssum + 1);
    cnt[abssum] = 1; // for the empty subarray

    int64_t ret{0};
    for (int i = 0; i < N; ++i) {
        prefsum += g_input[i];
        const auto addr = abssum + prefsum;
        for (const int s : g_squares) {
            if (addr < s) break;
            ret += cnt[addr - s];
        }
        ++cnt[addr];
    }

    std::cout << ret;
}

static void solve_set1() {
    int N; std::cin >> N;
    g_input.resize(N);
    for (auto& n : g_input)
        std::cin >> n;

    g_partial.resize(N + 1);
    std::partial_sum(g_input.begin(), g_input.end(), g_partial.begin() + 1);

    int64_t ret{0};
    for (int len = 1; len <= N; ++len)
        for (int pos = 0; pos + len <= N; ++pos) {
            const auto ran = range_sum(pos, pos + len - 1);
            const auto can = int64_t(std::sqrt(ran));
            if (can * can == ran)
                ++ret;
        }

    std::cout << ret;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    g_squares.reserve(int(std::sqrt(MAXSUM) + 1));
    for (int n = 0, s = 0; s < MAXSUM; ++n, s = n * n)
        g_squares.push_back(s);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 perfect_subarray.cpp -o perfect_subarray.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address perfect_subarray.cpp -o perfect_subarray

Run:
perfect_subarray.exe < perfect_subarray.in

Input:

3
3
2 2 6
5
30 30 9 1 30
4
4 0 0 16

Output:

Case #1: 1
Case #2: 3
Case #3: 9

*/


#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

// Milk Tea
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008f4332/0000000000943934

static int solve_set1() {
    int N, M, P;
    std::cin >> N >> M >> P;
    const int MAX = 1 << P;
    assert(P < 64 && M < MAX);
    std::vector<std::string> orders(N);
    for (auto& o : orders)
        std::cin >> o;
    std::unordered_set<int64_t> bad;
    for (int z = M; z; --z) {
        std::string s;
        std::cin >> s;
        int64_t b{0};
        for (const char c : s)
            b = (b << 1) + c - '0';
        bad.insert(b);
    }

    int64_t ideal{0};
    std::vector<int> misses(P); // bit misses for dominant answer
    std::vector<std::pair<int, int>> biases(P); // {bias, position}
    for (int p = 0; p < P; ++p) {
        int zero{0};
        for (int o = 0; o < N; ++o)
            if (orders[o][p] == '0')
                ++zero;
        ideal <<= 1;
        if (2 * zero < N) {
            ++ideal;
            misses[p] = zero;
        } else
            misses[p] = N - zero;
        biases[p] = {std::abs(N - 2 * zero), p};
    }

    std::sort(biases.begin(), biases.end());
    std::vector<int64_t> masks(P);
    for (int i = 0; i < P; ++i)
        masks[i] = 1 << (P - biases[i].second - 1);
    int64_t ans = 1e18;
    for (int64_t bias = 0; bias < MAX; ++bias) { // it is guarateed to end
        auto can = ideal, tmp = bias;
        for (int b = 0; tmp; ++b, tmp >>= 1)
            if (tmp & 1)
                can ^= masks[b];
        if (bad.count(can) == 0) {
            can = 0, tmp = bias;
            for (int b = 0; b < P; ++b, tmp >>= 1) {
                const auto d = misses[biases[b].second];
                can += (tmp & 1) ? N - d : d;
            }
            ans = std::min(ans, can);
        }
    }

    return ans;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": " << solve_set1() << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 milk_tea.cpp -o milk_tea.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address milk_tea.cpp -o milk_tea

Run:
milk_tea.exe < milk_tea.in

Input:

2
3 1 4
1100
1010
0000
1000
2 4 4
1111
1111
1111
0111
1011
1101

Output:

Case #1: 4
Case #2: 2

*/

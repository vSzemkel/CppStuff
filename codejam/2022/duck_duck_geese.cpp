
#include <iostream>
#include <vector>

// Duck, Duck, Geese
// https://codingcompetitions.withgoogle.com/codejam/round/00000000008779b4/0000000000b45244


static void solve_set1() { // some optimizations O(N*N); real 0m0.222s
    int N, C;
    std::cin >> N >> C;
    std::vector<int> lima(C), limb(C), hats(N);
    for (int i = 0; i < C; ++i)
        std::cin >> lima[i] >> limb[i];
    for (int i = 0; i < N; ++i) {
        std::cin >> hats[i];
        --hats[i];
    }

    std::vector<std::vector<int64_t>> prefsum(C, std::vector<int64_t>(2 * N + 1));
    for (int c = 0; c < C; ++c) {
        auto& pfs = prefsum[c];
        for (int i = 0; i < 2 * N; ++i) {
            pfs[i + 1] = pfs[i];
            if (hats[i % N] == c)
                ++pfs[i + 1];
        }
    }

    int64_t ans{0};
    for (int b = 0; b < N; ++b) {
        const int b_color = hats[b];
        if (limb[b_color] == 0)
            continue;
        int invalid_colors = (lima[b_color] <= 1) ? 0 : 1;
        for (int len = 2; len < N; ++len) {
            const int e = b + len - 1;
            const int c = hats[e % N];
            const auto& pref = prefsum[c];
            const int cnt = pref[e + 1] - pref[b];
            if (limb[c] < cnt)
                break;
            if (cnt == 1 && lima[c] > 0)
                ++invalid_colors;
            if (lima[c] == cnt)
                --invalid_colors;
            if (!invalid_colors)
                ++ans;
        }
    }

    std::cout << ans;
}

static void solve_set0() { // brute force O(N*N*C); real 0m10.122s
    int N, C;
    std::cin >> N >> C;
    std::vector<int> lima(C), limb(C), hats(N);
    for (int i = 0; i < C; ++i)
        std::cin >> lima[i] >> limb[i];
    for (int i = 0; i < N; ++i) {
        std::cin >> hats[i];
        --hats[i];
    }

    std::vector<std::vector<int64_t>> prefsum(C, std::vector<int64_t>(2 * N + 1));
    for (int c = 0; c < C; ++c) {
        auto& pfs = prefsum[c];
        for (int i = 0; i < 2 * N; ++i) {
            pfs[i + 1] = pfs[i];
            if (hats[i % N] == c)
                ++pfs[i + 1];
        }
    }

    int64_t ans{0};
    for (int b = 0; b < N; ++b)
        for (int len = 2; len < N; ++len) {
            bool ok{true};
            const int e = b + len - 1;
            for (int c = 0; ok && c < C; ++c) {
                const int cnt = prefsum[c][e + 1] - prefsum[c][b];
                ok = !cnt || (lima[c] <= cnt && cnt <= limb[c]);
            }
            if (ok)
                ++ans;
        }

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
        std::cout << "Case #" << g << ": "; solve_set1(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 duck_duck_geese.cpp -o duck_duck_geese.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address duck_duck_geese.cpp -o duck_duck_geese

Run:
duck_duck_geese.exe < duck_duck_geese.in

Input:

3
3 2
1 1
1 1
1 1 2
5 2
1 1
1 2
1 2 1 2 2
3 3
1 2
1 2
2 2
1 1 3

Output:


*/

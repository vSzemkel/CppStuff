
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

// Trash Bins
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000435bae/0000000000887c32

inline static int64_t ps(const int64_t n){ return n * (n + 1) / 2; };

static void solve() {
    int64_t N; std::string S;
    std::cin >> N >> S;

    size_t lpos = S.find('1');
    size_t rpos = S.rfind('1');
    int64_t ret = ps(lpos) + ps(N - rpos - 1);

    while (lpos < rpos) {
        const int64_t mpos = S.find('1', lpos + 1);
        const int64_t len = mpos - lpos - 1;
        const int64_t half = len / 2;
        ret += 2 * ps(half);
        if (len & 1)
            ret += half + 1;
        lpos = mpos;
    }

    std::cout << ret;
}

static void solve2() { // 2x SLOWER then solve()
    int64_t N; std::string S;
    std::cin >> N >> S;

    std::vector<int> dist(N);
    std::transform(S.begin(), S.end(), dist.begin(), [](const char c){ return c == '1' ? 0 : 1000000; });
    for (int i = 1; i < N; ++i)
        dist[i] = std::min(dist[i], dist[i - 1] + 1);
    for (int i = N - 2; i >= 0; --i)
        dist[i] = std::min(dist[i], dist[i + 1] + 1);
    auto ret = std::accumulate(dist.begin(), dist.end(), 0LL);
    std::cout << ret;
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 trash_bins.cpp -o trash_bins.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address trash_bins.cpp -o trash_bins

Run:
trash_bins.exe < trash_bins.in

Input:

2
6
100100
3
111

Output:

Case #1: 5
Case #2: 0

*/

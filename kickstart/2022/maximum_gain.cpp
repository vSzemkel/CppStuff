
#include <iostream>
#include <numeric>
#include <vector>

// Maximum Gain
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008caea6/0000000000b76fae

static void solve() {
    int N; std::cin >> N;
    std::vector<int64_t> AN(N);
    for (auto& v : AN)
        std::cin >> v;
    int M; std::cin >> M;
    std::vector<int64_t> BM(M);
    for (auto& v : BM)
        std::cin >> v;
    int K; std::cin >> K;

    std::vector<int64_t> ANP(N + 1);
    std::partial_sum(AN.begin(), AN.end(), ANP.begin() + 1);
    std::vector<int64_t> BMP(M + 1);
    std::partial_sum(BM.begin(), BM.end(), BMP.begin() + 1);

    int64_t ans{0};
    for (int i = 0; i <= K; ++i) // take i items from AN
        if (i <= N && K - i <= M) {
            int64_t canA{0}, canB{0};
            for (int j = 0; j <= i; ++j) // take j items from left of AN and i-j from right
                canA = std::max(canA, ANP[j] + ANP[N] - ANP[N - (i - j)]);
            for (int j = 0; j <= K - i; ++j) // take j items from left of BM and K-i-j from right
                canB = std::max(canB, BMP[j] + BMP[M] - BMP[M - (K - i - j)]);
            ans = std::max(ans, canA + canB);
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
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 maximum_gain.cpp -o maximum_gain.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address maximum_gain.cpp -o maximum_gain

Run:
maximum_gain.exe < maximum_gain.in

Input:

2
3
3 1 2
4
2 8 1 9
5
4
1 100 4 3
6
15 10 12 5 1 10
6

Output:

Case #1: 24
Case #2: 148

*/


#include <iostream>
#include <vector>

// Walktober
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb2e1/0000000000c174f2

static void solve() {
    int M, N, P;
    std::cin >> M >> N >> P;
    std::vector<std::vector<int64_t>> scores(M + 1, std::vector<int64_t>(N));

    for (int m = 1; m <= M; ++m) 
        for (int d = 0; d < N; ++d) {
            auto& s = scores[m][d];
            auto& h = scores[0][d];
            std::cin >> s;
            h = std::max(h, s);
        }

    int64_t ans{0};
    for (int d = 0; d < N; ++d)
        if (scores[0][d] > scores[P][d])
            ans += scores[0][d] - scores[P][d];

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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 walktober.cpp -o walktober.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address walktober.cpp -o walktober

Run:
walktober.exe < walktober.in

Input:

2
3 2 3
1000 2000
1500 4000
500 4000
3 3 2
1000 2000 1000
1500 2000 1000
500 4000 1500

Output:

Case #1: 1000
Case #2: 2500

*/

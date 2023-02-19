
#include <iostream>
#include <vector>

// Shifts
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050e02/000000000018fd5e


static void solve() { // by cerberus97
    int N, H;
    std::cin >> N >> H;
    std::vector<int> A(N), B(N);
    for (auto& a : A)
        std::cin >> a;
    for (auto& b : B)
        std::cin >> b;

    // check if B quarding at zeros will be happy
    const auto MX = 1 << N;
    std::vector<int64_t> absentB(MX);
    for (int m = 0; m < MX; ++m) {
        int64_t sum{};
        for (int i = 0; i < N; ++i)
            if ((m & (1 << i)) == 0)
                sum += B[i];
        if (H <= sum)
            absentB[m] = 1;
    }
    // count possibilities for B guarding zeros and some ones be happy
    for (int i = 0; i < N; ++i)
        for (int m = 0; m < MX; ++m)
            if (m & (1 << i))
                absentB[m] += absentB[m ^ (1 << i)];
    // for every happy A take complement of B
    int64_t ans{};
    for (int m = 0; m < MX; ++m) {
        int64_t sum{};
        for (int i = 0; i < N; ++i)
            if (m & (1 << i))
                sum += A[i];
        if (H <= sum)
            ans += absentB[m];
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
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++17 shifts.cpp -o shifts.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address shifts.cpp -o shifts

Run:
shifts.exe < shifts.in

Input:

2
2 3
1 2
3 3
2 5
2 2
10 30

Output:

Case #1: 3
Case #2: 0

*/

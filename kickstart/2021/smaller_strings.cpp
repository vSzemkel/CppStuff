
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <string>

// Smaller strings
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000435c44/00000000007ebe5e

constexpr int M = 1'000'000'007;

static void solve() {
    int64_t N, K; std::string S;
    std::cin >> N >> K >> S;
    assert(int(S.size()) == N);

    const int has_center = N & 1;
    const int L = N / 2 + has_center;
    auto rev = S.substr(0, L);
    std::reverse(rev.begin(), rev.end());
    const int growing_bonus = (rev < S.substr(L - has_center)) ? 1 : 0;
    S.resize(L);

    int64_t ret{0};
    for (const char c : S)
        ret = (ret * K + (c - 'a')) % M;

    std::cout << (M + ret + growing_bonus) % M;
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 smaller_strings.cpp -o smaller_strings.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address smaller_strings.cpp -o smaller_strings

Run:
smaller_strings.exe < smaller_strings.in

Input:

7
2 3
cc
6 2
baaaaa
3 2
bab
2 3
bc
5 5
abcdd
1 5
d
15 26
zzzzzzzzzzzzzzz

Output:

Case #1: 2
Case #2: 4
Case #3: 2
Case #4: 2
Case #5: 8
Case #6: 3
Case #7: 827063119

*/

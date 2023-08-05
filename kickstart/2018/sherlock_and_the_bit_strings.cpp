
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

// Sherlock and the Bit Strings
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050ff4/000000000005107b

static void solve_set1() {
    int64_t N, K, P;
    std::cin >> N >> K >> P;

    P = std::min(P, int64_t{1e18}) - 1;
    std::string ans(N, '0');
    std::vector<bool> masked(N);
    for (int z = K; z; --z) {
        int A, B, C;
        std::cin >> A >> B >> C;
        assert(A == B && C < 2);
        masked[--A] = true;
        if (C > 0)
            ans[A] = '1';
    }

    // UB prevention with check <64 is critical for corectness here
    for (int64_t bit = 0, ansbit = N - 1; ~ansbit && bit < 64; --ansbit)
        if (!masked[ansbit]) {
            if (P & (uint64_t(1) << bit))
                ans[ansbit] = '1';
            ++bit;
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
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 sherlock_and_the_bit_strings.cpp -o sherlock_and_the_bit_strings.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address sherlock_and_the_bit_strings.cpp -o sherlock_and_the_bit_strings

Run:
sherlock_and_the_bit_strings.exe < sherlock_and_the_bit_strings.in

Input:

2
10 7 7
10 10 0
6 6 0
7 7 0
5 5 1
1 1 0
8 8 1
3 3 0
95 13 565153962463418788
68 68 1
71 71 1
25 25 0
12 12 0
80 80 1
5 5 0
42 42 1
17 17 1
75 75 0
2 2 0
3 3 1
19 19 1
79 79 1

Output:

Case #1: 0101100100
Case #2: 00100000000000001010000000000011111010111111010100100010001101100001001010001011101100110100011

*/

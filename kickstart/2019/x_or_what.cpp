
#include <iostream>
#include <set>
#include <vector>

// X or What
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000051061/0000000000161426

static bool is_xor_even(const int x) {
    return (__builtin_popcount(x) & 1) == 0;
}

/**
 * Observation1: xor_odd ^ xor_odd == xor_even ^ xor_even == xor_even
 * Observation2: xor_odd ^ xor_even == xor_even ^ xor_odd == xor_odd
 * Observation3: largest xor_even subarray has even number of xor_odd numbers
 */
static void solve() {
    int N, Q;
    std::cin >> N >> Q;
    std::set<int> xor_odd;
    std::vector<int> input(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> input[i];
        if (!is_xor_even(input[i]))
            xor_odd.insert(i);
    }

    for (int z = Q; z; --z) {
        int pos, val;
        std::cin >> pos >> val;
        xor_odd.erase(pos);
        if (!is_xor_even(val))
            xor_odd.insert(pos);
        input[pos] = val;

        if (xor_odd.size() % 2 == 0) {
            std::cout << N << ' ';
            continue;
        }

        std::cout << std::max(*xor_odd.rbegin(), N - *xor_odd.begin() - 1) << ' ';
    }
}

static void solve_set1() {
    int N, Q, acc{0};
    std::cin >> N >> Q;
    std::vector<int> input(N);
    for (auto& n : input) {
        std::cin >> n;
        acc ^= n;
    }

    for (int z = Q; z; --z) {
        int pos, val;
        std::cin >> pos >> val;
        acc ^= input[pos] ^ val;
        input[pos] = val;

        if (is_xor_even(acc)) {
            std::cout << N << ' ';
            continue;
        }

        bool found{};
        auto head = acc;
        for (int len = N - 1; !found && len; --len) {
            head ^= input[len];
            auto cur = head;
            for (int begin = 0; begin + len  <= N; ++begin) {
                if (is_xor_even(cur)) {
                    found = true;
                    std::cout << len << ' ';
                    break;
                }

                cur ^= input[begin] ^ input[begin + len];
            }
        }

        if (!found)
            std::cout << "0 ";
    }
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 x_or_what.cpp -o x_or_what.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address x_or_what.cpp -o x_or_what

Run:
x_or_what.exe < x_or_what.in

Input:

2
4 3
10 21 3 7
1 13
0 32
2 22
5 1
14 1 15 20 26
4 26

Output:

Case #1: 4 3 4
Case #2: 4

*/

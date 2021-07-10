
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

// Mural
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000051060/0000000000058b89

std::vector<int> g_partial;

int64_t partial_sum(const int i, const int j) { return g_partial[j + 1] - g_partial[i]; }

static void solve() {
    int N; std::cin >> N;
    std::vector<int> input(N);
    for (int i = 0; i < N; ++i) {
        char c; std::cin >> c;
        input[i] = c - '0';
    }

    g_partial.resize(N + 1, 0);
    std::partial_sum(input.begin(), input.end(), g_partial.begin() + 1);

    int64_t ret{0};
    const int size = (N + 1) / 2;
    for (int i = 0; i < N - size + 1; ++i)
        ret = std::max(ret, partial_sum(i, i + size - 1));

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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 mural.cpp -o mural.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address mural.cpp -o mural

Run:
mural.exe < mural.in

Input:

4
4
1332
4
9583
3
616
10
1029384756

Output:

Case #1: 6
Case #2: 14
Case #3: 7
Case #4: 31

*/

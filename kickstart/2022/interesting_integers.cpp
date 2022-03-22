
#include <algorithm>
#include <array>
#include <assert.h>
#include <bitset>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <filesystem>
#include <fstream>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <stdlib.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <vector>

// Interesting Integers
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb33e/00000000009e73ea


static void solve() { // TLE
    int64_t A, B;
    std::cin >> A >> B;

    int ret{0};
    for (auto n = A; n <= B; ++n) {
        int64_t k{n}, m{1}, s{0};
        while (k) {
            const int d = k % 10;
            m *= d;
            s += d;
            k /= 10;
        }
        if ((m % s) == 0)
            ++ret;
    }

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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 interesting_integers.cpp -o interesting_integers.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address interesting_integers.cpp -o interesting_integers

Run:
interesting_integers.exe < interesting_integers.in

Input:

4
1 9
91 99
451 460
501 1000

Output:

Case #1: 9
Case #2: 0
Case #3: 5
Case #4: 176

*/

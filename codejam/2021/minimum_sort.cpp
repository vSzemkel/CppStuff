
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

// Minimum Sort
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000435915/00000000007dc51c

int N;

static void solve() {
    for (int i = 1; i < N; ++i) {
        std::cout << "M " << i << ' ' << N << std::endl;
        int j; std::cin >> j;
        if (i < j) {
            std::cout << "S " << i << ' ' << j << std::endl;
            std::cin >> j; assert(j == 1);
        }
    }

    std::cout << 'D' << std::endl;
    int ans; std::cin >> ans;
    assert(ans == 1);
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases >> N;
    for (int g = 1; g <= no_of_cases; ++g) {
        solve();
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 minimum_sort.cpp -o minimum_sort.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address minimum_sort.cpp -o minimum_sort

Run:
py.exe interactive_runner.py py.exe minimum_sort_testing_tool.py 1 -- minimum_sort.exe

*/


#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <vector>

// Next Greater
// For every array element i, find j : a[i] < a[j] or -1 if not exists

constexpr int g_size = 100000;
std::array<int, g_size> g_input, g_result;

const auto rand_in_range = [](const int ubound){std::random_device seed;std::mt19937 gen{seed()};std::uniform_int_distribution<int> dist(0,ubound-1);return dist(gen);};

static void solve() {
    std::vector<int> stack;
    for (int i = 0; i < g_size; ++i) {
        while (!stack.empty() && g_input[stack.back()] < g_input[i]) {
            g_result[stack.back()] = i;
            stack.pop_back();
        }
        stack.push_back(i);
    }
    while (!stack.empty()) {
        g_result[stack.back()] = -1;
        stack.pop_back();
    }
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    const auto rgen = [](){ return rand_in_range(1000000); };
    std::generate_n(g_input.begin(), g_size, rgen);

    solve();
    for (int i = 0; i < std::min(20, g_size); ++i) {
        std::cout << g_input[i] << " ";
        if (g_result[i] > 0)
            std::cout << "< " << g_input[g_result[i]] << "   ";
        std::cout << i << " -> " << g_result[i] << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 next_greater.cpp -o next_greater.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 next_greater.cpp -o next_greater.o

*/

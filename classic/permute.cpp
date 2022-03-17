
#include <array>
#include <iostream>
#include <numeric>

// Enumarate all permutations of g_size elements
// If elements are comparable use std::next_permutation


// case definition - words collection
constexpr int g_size = 5;

// globals
int g_counter = 1;
std::array<int, g_size> g_arr;

static void print_array() {
    std::cout << g_counter++ << ".\t[";
    for (auto it = g_arr.begin(); it < g_arr.end() - 1; ++it)
        std::cout << *it << ", ";
    std::cout << g_arr.back() << "]\n";
}

static void permute(const int size)
{
    if (size < 2) {
        print_array();
        return;
    }

    auto& last = g_arr.back();
    for (int i = size - 1; ~i; --i) {
        std::swap(g_arr[i], last);
        permute(size - 1);
        std::swap(g_arr[i], last);
    }
}

int main(int argc, char* argv[])
{
    std::iota(g_arr.begin(), g_arr.end(), 0);
    permute(g_size);
    return 0;
}

/* clang++.exe -Wall -g -std=c++17 permute.cpp -o permute.exe

Output ends with:

...
118.	[2, 3, 1, 4, 0]
119.	[2, 1, 3, 4, 0]
120.	[1, 2, 3, 4, 0]

*/
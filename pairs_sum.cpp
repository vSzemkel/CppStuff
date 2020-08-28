
#include <array>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <random>

// given an array and a number find all pairs of values 
// in the array that have sum equal to the number


// case definition
constexpr int g_size = 100;
constexpr int g_range = 1000;
constexpr int g_expected = 25;

// globals
std::array<int, g_size> g_arr;
std::unordered_map<int, int> g_candidates; // {arr[i], i}
std::vector<std::pair<int,std::pair<int, int>>> g_found; // {j, {arr[i], i}}

void init()
{
    std::random_device rd;
    std::uniform_int_distribution<int> dist(-g_range, g_range);
    for (int n = 0; n < g_size; ++n)
        g_arr[n] = dist(rd);
}

void find_pairs()
{
    for (int i = 0; i < g_size; ++i) {
        auto expected = g_expected - g_arr[i];
        const auto it = g_candidates.find(expected);
        if (it == g_candidates.end())
            g_candidates.emplace(g_arr[i], i);
        else {
            g_found.emplace_back(i, *it);
            g_candidates.erase(it);
        }
    }
}

int main(int argc, char* argv[])
{
    init();
    find_pairs();

    // present result
    printf("\nRecognized %llu pairs of required sum %i:\n", g_found.size(), g_expected);
    for (const auto& f : g_found) {
        const auto partial = f.second.first;
        printf("\tarr[%i] + arr[%i] = %i + %i = %i\n", f.first, f.second.second, g_expected - partial, partial, g_expected);
    }
}

/*    clang++.exe -Wall -g -std=c++17 pairs_sum.cpp -o pairs_sum.exe

Sample output:

Recognized 3 pairs of required sum 25:
	arr[14] + arr[11] = -396 + 421 = 25
	arr[71] + arr[44] = 121 + -96 = 25
	arr[98] + arr[77] = 113 + -88 = 25

*/

#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

// Equity swap
// Given two arrays of integers, find a pair of values (one value from each array)
// that you can swap to give the two arrays the same sum

bool g_swapped;
std::vector<int> g_a, g_b;

template <typename T> std::vector<T> fill(const size_t size){ std::vector<T> cont(size); std::copy_n(std::istream_iterator<T>{std::cin}, size, cont.begin()); return cont; };

static void print(int posa, int valb)
{
    int posb = std::find(g_b.begin(), g_b.end(), valb) - g_b.begin();
    if (g_swapped) std::swap(posa, posb);
    std::cout << "{" << posa << ", " << posb << "}";
}

static void solve() // O((M+N)log(min(M,N)))
{
    int sza, szb;
    std::cin >> sza >> szb;
    g_a = fill<int>(sza);
    g_b = fill<int>(szb);
    g_swapped = sza < szb;
    if (g_swapped) {
        std::swap(sza, szb);
        std::swap(g_a, g_b);
    }

    const int suma = std::accumulate(g_a.begin(), g_a.end(), 0);
    const int sumb = std::accumulate(g_b.begin(), g_b.end(), 0);

    int diff = suma - sumb;
    if (diff % 2 == 0) {
        diff >>= 1;
        auto ab = g_b;
        std::sort(ab.begin(), ab.end());
        for (int posa = 0; posa < sza; ++posa) {
            const auto vala = g_a[posa];
            const auto it = std::lower_bound(ab.begin(), ab.end(), vala - diff);
            if (it < ab.end() && diff == vala - *it) {
                print(posa, *it);
                return;
            }
        }
    }

    std::cout << "IMPOSSIBLE";
}

int main(int argc, char* argv[])
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    // parse console input
    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        // Set 1
        std::cout << "Case #" << g << ": "; solve(); std::cout << "\n";
    }
}

/*
clang++.exe -Wall -ggdb3 -O0 -std=c++17 equity_swap.cpp -o equity_swap.exe
g++ -Wall -ggdb3 -O0 -std=c++17 equity_swap.cpp -o equity_swap.o
equity_swap.exe < equity_swap.in

Input:

4
2 2
0 1
2 3
3 5
1 2 3
4 1 1 2 0
3 3
1 1 1
2 2 2
10 10
5 1 1 2 1 1 1 1 1 1
5 1 1 1 1 1 1 0 1 1

Output:

Case #1: {0, 0}
Case #2: {2, 0}
Case #3: IMPOSSIBLE
Case #4: {1, 7}

*/


#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

// Equity swap
// Given two arrays of integers, find a pair of values (one value from each array)
// that you can swap to give the two arrays the same sum

std::vector<int> g_a, g_b;

template <typename T> std::vector<T> fill(const size_t size){ std::vector<T> cont(size); std::copy_n(std::istream_iterator<T>{std::cin}, size, cont.begin()); return cont; };

void print(const int a, const int b)
{
    int posb{0};
    while (g_b[posb] != b) ++posb;
    std::cout << "{" << a << ", " << posb << "}";
}

void solve() // O((m+n)logm)
{
    int sza, szb;
    std::cin >> sza >> szb;
    g_a = fill<int>(sza);
    g_b = fill<int>(szb);
    auto ab = g_b;

    const int suma = std::accumulate(g_a.begin(), g_a.end(), 0);
    const int sumb = std::accumulate(g_b.begin(), g_b.end(), 0);

    int diff = suma - sumb;
    if (diff % 2 == 0) {
        diff >>= 1;
        std::sort(ab.begin(), ab.end());
        for (int posa = 0; posa < sza; ++posa) {
            const auto vala = g_a[posa];
            const auto it1 = std::lower_bound(ab.begin(), ab.end(), vala - diff);
            if (it1 < ab.end() && diff == vala - *it1) {
                print(posa, *it1);
                return;
            }
            const auto it2 = std::lower_bound(ab.begin(), ab.end(), vala + diff);
            if (it2 < ab.end() && diff == *it2 - vala) {
                print(posa, *it2);
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
Case #2: {0, 4}
Case #3: IMPOSSIBLE
Case #4: {1, 7}

*/


#include <iostream>
#include <numeric>
#include <unordered_map>
#include <vector>

// Equity swap
// Given two arrays of integers, find a pair of values (one value from each array)
// that you can swap to give the two arrays the same sum

std::vector<int> g_a, g_b;

/**
 * suma - a + b = sumb - b + a
 * a - b = (suma - sumb) / 2
 * find b: b = a - (suma - sumb) / 2
 */
static void solve() // O(M + N)
{
    int sza, szb;
    std::cin >> sza >> szb;
    g_a.resize(sza);
    for (auto& a : g_a)
        std::cin >> a;
    g_b.resize(szb);
    for (auto& b : g_b)
        std::cin >> b;

    const int suma = std::accumulate(g_a.begin(), g_a.end(), 0);
    const int sumb = std::accumulate(g_b.begin(), g_b.end(), 0);

    int diff = suma - sumb;
    if (diff % 2 == 0) {
        diff >>= 1;
        std::unordered_map<int, int> mapb;
        for (int i = szb - 1; ~i; --i)
            mapb[g_b[i]] = i;
        for (int posa = 0; posa < sza; ++posa) {
            const auto it = mapb.find(g_a[posa] - diff);
            if (it != mapb.end()) {
                std::cout << "Swap A[" << posa << "] and B[" << it->second << "]";
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
4 1 1 2 0 3 3 5 1 1

Output:

Case #1: Swap A[0] and B[0]
Case #2: Swap A[0] and B[3]
Case #3: IMPOSSIBLE
Case #4: Swap A[1] and B[0]

*/

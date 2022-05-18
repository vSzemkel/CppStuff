
#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdlib>
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
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <vector>

// Diverse Subarray
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050eda/00000000001198c1#problem


static void solve_set1() { // O(N2)
    int N;
    size_t S;
    std::cin >> N >> S;
    std::vector<int> trinkets(N);
    std::unordered_map<int, std::vector<int>> pos; // type -> sorted list of positions
    for (int i = 0; i < N; ++i) {
        int t; std::cin >> t;
        trinkets[i] = t;
        pos[t].push_back(i);
    }

    size_t ans{0};
    for (int i = 0; i < N; ++i) {
        std::unordered_set<int> omit;
        std::unordered_multiset<int> cur;
        for (int j = i; j < N; ++j) {
            const int t = trinkets[j];
            if (omit.count(t))
                continue;
            if (cur.count(t) < S)
                cur.insert(t);
            else {
                cur.erase(t);
                omit.insert(t);
            }

            ans = std::max(ans, cur.size());
        }
    }

    std::cout << ans;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve_set1(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 diverse_subarray.cpp -o diverse_subarray.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address diverse_subarray.cpp -o diverse_subarray

Run:
diverse_subarray.exe < diverse_subarray.in

Input:

4
6 2
1 1 4 1 4 4
8 1
1 2 500 3 4 500 6 7
10 1
100 200 8 8 8 8 8 300 400 100
12 2
40 50 1 1 1 60 70 2 2 2 80 90

Output:

Case #1: 4
Case #2: 6
Case #3: 4
Case #4: 6

*/

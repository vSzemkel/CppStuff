
#include <algorithm>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

// Voivodeships
// After merging two voivodeships the name of one with heighest population is preserved
// Ties break at random. Which voivodeships names can eventually be preserved after full merge?

static void solve() {
    int pos{0}, N{0};
    std::cin >> N;
    std::vector<int> partial(N);
    std::vector<std::pair<int, int>> vships(N);
    for (auto& v : vships) {
        std::cin >> v.first;
        v.second = ++pos;
    }

    std::sort(vships.begin(), vships.end());
    std::transform(vships.begin(), vships.end(), partial.begin(), [](const auto& v){ return v.first; });
    std::partial_sum(partial.begin(), partial.end(), partial.begin());

    int ret{N - 1};
    const int MAX = vships.back().first;
    for (int i = ret - 1; i >= 0; --i)
        if (partial[i] >= MAX)
            --ret;
        else
            break;

    std::sort(vships.begin() + ret, vships.end(), [](const auto& v1, const auto& v2){
        return v1.second < v2.second;
    });
    
    for (int i = ret; i < N; ++i)
        std::cout << vships[i].second << ' ';
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 voivodeships.cpp -o voivodeships.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address voivodeships.cpp -o voivodeships.o

Run:
voivodeships.exe < voivodeships.in

Input:

4
3
1 1 1
4
5 990 8 999
5
10 3 2 6 7
10
3 5 12 7 97 23 4 67 11 41

Output:

Case #1: 1 2 3 
Case #2: 2 4
Case #3: 1 4 5
Case #4: 5 8 10

*/

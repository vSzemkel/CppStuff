
#include <algorithm>
#include <iostream>
#include <format>
#include <vector>

// Line of Delivery A
// https://www.facebook.com/codingcompetitions/hacker-cup/2024/practice-round/problems/D1

static void solve() // O(N)
{
    int N, G;
    std::cin >> N >> G;

    int index{0};
    int left = std::numeric_limits<int>::min();
    int right = std::numeric_limits<int>::max();
    for (int i = 0; i < N; ++i) {
        int s; std::cin >> s;
        if (s < G) {
            ++index;
            if (left < s)
                left = s;
        } else if (s < right)
            right = s;
    }

    if (index == 0)
        std::cout << std::format("{} {}", N, right - G);
    else if (index == N)
        std::cout << std::format("1 {}", G - left);
    else {
        if (G - left < right - G)
            std::cout << std::format("{} {}", N - index + 1, G - left);
        else
            std::cout << std::format("{} {}", N - index, right - G);
    }
}

static void solve_nlogn() // O(N*log(N))
{
    int N, G;
    std::cin >> N >> G;
    std::vector<int> stones(N);
    for (auto& s : stones)
        std::cin >> s;

    std::sort(stones.begin(), stones.end());
    const auto it = std::lower_bound(stones.begin(), stones.end(), G);
    if (it == stones.end())
        std::cout << std::format("1 {}", G - stones.back());
    else if (*it == G)
        std::cout << std::format("{} 0", int(stones.end() - it));
    else if (it == stones.begin())
        std::cout << std::format("{} {}", N, stones.front() - G);
    else {
        const int right = int(it - stones.begin());
        const int left = right - 1;
        if (G - stones[left] < stones[right] - G)
            std::cout << std::format("{} {}", N - left, G - stones[left]);
        else
            std::cout << std::format("{} {}", N - right, stones[right] - G);
    }
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << std::format("Case #{}: ", g);
        solve();
        std::cout << '\n';
    }
}

/*

Format:
clang-format -i line_of_delivery_a.cpp

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 line_of_delivery_a.cpp -o line_of_delivery_a.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address line_of_delivery_a.cpp -o line_of_delivery_a

Run:
line_of_delivery_a.exe < line_of_delivery_a.in

Input:

4
2 5
7
2
3 1
9
5
7
4 7
8
7
9
6
2 10
15
5

Output:

Case #1: 1 2
Case #2: 3 4
Case #3: 3 0
Case #4: 1 5

*/

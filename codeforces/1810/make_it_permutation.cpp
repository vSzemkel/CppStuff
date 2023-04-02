
#include <algorithm>
#include <iostream>
#include <span>
#include <vector>

// Make It Permutation
// https://codeforces.com/contest/1810/problem/C

int64_t N, R, I;

static int64_t solve_inner(std::span<int> data, int last_good) {
    if (data.empty())
        return 0;

    const auto next = std::span<int>{data.data() + 1, data.size() - 1};
    if (data.front() == last_good)
        return R + solve_inner(next, last_good);
    if (data.front() == last_good + 1)
        return solve_inner(next, last_good + 1);

    return std::min(data.size() * R, size_t(data.front() - last_good - 1) * I + solve_inner(next, data.front()));
}

static void solve() {
    std::cin >> N >> R >> I;
    std::vector<int> data(N);
    for (auto& d : data)
        std::cin >> d;

    std::sort(data.begin(), data.end());

    int cost{}, start{};
    if (data.front() != 1) {
        cost += I;
        ++start;
    }

    std::cout << cost + solve_inner(data, start);
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        solve(); std::cout << '\n';
    }
}

/*

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 make_it_permutation.cpp -o make_it_permutation.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address make_it_permutation.cpp -o make_it_permutation

Run:
make_it_permutation.exe < make_it_permutation.in

Input:

8
3 3 3
1 2 3
5 1 5
1 2 3 5 6
5 2 3
1 1 1 3 3
5 1 10
2 4 6 8 10
6 2 8
7 3 5 4 4 8
4 10 1
1 2 6 7
4 3 3
2 5 8 7
2 1000000000 1
1000000000 1

Output:

Case #1: 0
Case #2: 2
Case #3: 8
Case #4: 14
Case #5: 20
Case #6: 3
Case #7: 12
Case #8: 999999998

*/

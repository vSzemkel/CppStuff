
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <vector>

// Good subarrays
// https://codeforces.com/contest/1398/problem/C

std::vector<int> g_input;

// sum[i..j] = partial[j+1] - partial[i] == j - i + 1 == (j+1) - i
// partial[j+1] - (j+1) == partial[i] - i
static void solve() {
    int N; std::cin >> N;
    g_input.resize(N);
    for (auto& n : g_input) {
        char c; std::cin >> c;
        n = c & 0xf;
    }

    int sum{0};
    std::unordered_map<int, int> counter;
    counter[0] = 1;
    for (int i = 0; i < N; ++i) {
        sum += g_input[i];
        ++counter[sum - i - 1];
    }

    // if counter[x] == N, then we can choose (N-1 2) distinct pairs, eg good subarrays
    int64_t ret{0};
    for (const auto& c : counter)
        ret += int64_t(c.second - 1) * c.second;
    std::cout << ret / 2 << '\n';
}

static void solve_usemem() {
    int N; std::cin >> N;
    g_input.resize(N);
    for (auto& n : g_input) {
        char c; std::cin >> c;
        n = c & 0xf;
    }

    std::vector<int> g_partial(N + 1);
    g_partial[0] = 0;
    std::partial_sum(g_input.begin(), g_input.end(), g_partial.begin() + 1);

    std::unordered_map<int, int> counter;
    for (int i = 0; i <= N; ++i)
        ++counter[g_partial[i] - i];

    // if counter[x] == N, then we can choose (N 2) distinct pairs, eg good subarrays
    int64_t ret{0};
    for (const auto& c : counter)
        ret += int64_t(c.second - 1) * c.second / 2;
    std::cout << ret << '\n';
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        solve();
        //solve_usemem();
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 good_subarrays.cpp -o good_subarrays.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address good_subarrays.cpp -o good_subarrays

Run:
good_subarrays.exe < good_subarrays.in

Input:

4
3
120
5
11011
6
600005
100000
1111111111111111111...

Output:

3
6
1
5000050000

*/

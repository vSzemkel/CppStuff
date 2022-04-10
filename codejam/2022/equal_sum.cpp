
#include <iostream>
#include <numeric>
#include <vector>

// Equal Sum
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000877ba5/0000000000aa8fc1

constexpr const int MAX = 1e09;

/**
 * @brief My numbers sum must be positive
 * Last numbers of naive fair 2-partition algorithm should be decreasing powers of 2
 */
static void solve() {
    int N; std::cin >> N;
    std::vector<int> data;
    data.reserve(N);
    for (int i = 1; i <= MAX; i <<= 1)
        data.push_back(i);
    const int special = int(data.size());
    for (int z = N - special, i = 5000; z; --z)
        data.push_back(i++);

    int64_t sum = std::accumulate(data.begin(), data.end(), 0LL);
    if (sum & 1) {
        ++sum;
        ++data.back();
    }

    for (const int i : data)
        std::cout << i << ' ';
    std::cout << std::endl;
    for (int z = N; z; --z) {
        int d; std::cin >> d;
        data.push_back(d);
    };

    std::vector<int> ans;
    int64_t taken{0}, nottaken{0};
    for (int i = special; i < 2 * N; ++i)
        if (taken < nottaken) {
            ans.push_back(data[i]);
            taken += data[i];
        } else
            nottaken += data[i];

    for (int i = special - 1; ~i; --i)
        if (taken < nottaken) {
            ans.push_back(data[i]);
            taken += data[i];
        } else
            nottaken += data[i];

    for (const int t : ans)
        std::cout << t << ' ';
    std::cout << std::endl;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g)
        solve();
}

/*

Compile:
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 equal_sum.cpp -o equal_sum.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address equal_sum.cpp -o equal_sum

Run:
py.exe interactive_runner.py py.exe equal_sum_testing_tool.py 1 -- equal_sum.exe

*/

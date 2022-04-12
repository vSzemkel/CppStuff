
#include <iostream>
#include <vector>

// Equal Sum
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000877ba5/0000000000aa8fc1

constexpr const int MAGIC = 600; // there is no power of 2 in range [MAGIC..MAGIC+100]
constexpr const int MAX = 1e09;

/**
 * Last numbers of naive fair 2-partition algorithm should be decreasing powers of 2
 */
static void solve() {
    int N; std::cin >> N;
    std::vector<int> data;
    data.reserve(N);
    for (int i = 1; i <= MAX; i <<= 1)
        data.push_back(i);
    const int special = int(data.size());
    for (int z = N - special, i = MAGIC; z; --z, ++i)
        data.push_back(i);

    for (const int i : data)
        std::cout << i << ' ';
    std::cout << std::endl;
    for (int z = N; z; --z) {
        int d; std::cin >> d;
        data.push_back(d);
    };
    // assert(sum(data) % 2 == 0)

    int balance{0};
    std::vector<int> ans;
    const auto naive = [&](const int v) {
        if (balance < 0) {
            ans.push_back(v);
            balance += v;
        } else
            balance -= v;
    };

    for (int i = special; i < 2 * N; ++i) naive(data[i]);
    for (int i = special - 1; ~i; --i)  naive(data[i]);

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

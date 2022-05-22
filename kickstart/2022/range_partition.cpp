
#include <iostream>
#include <set>

// Range Partition
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb4d1/0000000000b20deb


static void solve() {
    int64_t N, X, Y;
    std::cin >> N >> X >> Y;

    const int64_t sum = N * (N + 1) / 2;
    // A + B == sum
    // A / (sum - A) == X / Y
    // A * Y == X * (sum - A)
    // A (X + Y) = X * sum
    // A = X * sum / (X + Y);

    const auto num = X * sum, denom = X + Y;
    if (num % denom != 0) {
        std::cout << "IMPOSSIBLE";
        return;
    }

    int suma{0};
    std::set<int> as, bs;
    const auto A = num / denom;
    for (int i = N; i > 0; --i)
        if (suma + i <= A) {
            suma += i;
            as.insert(i);
        } else
            bs.insert(i);

    std::cout << "POSSIBLE\n" << as.size() << '\n';
    for (const int i : as)
        std::cout << i << ' ';
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
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 range_partition.cpp -o range_partition.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address range_partition.cpp -o range_partition

Run:
range_partition.exe < range_partition.in

Input:

3
3 1 2
3 1 1
3 1 3

Output:

Case #1: POSSIBLE
1
2
Case #2: POSSIBLE
1
3
Case #3: IMPOSSIBLE

*/

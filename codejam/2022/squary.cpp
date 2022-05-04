
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

// Squary
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000877b42/0000000000afdf76


/**
 * @brief Let E be initial sequence
 * Let S(seq) be a sum of sequence elements, eg S(a, b, c) = a + b + c
 * P(seq) be a pairwise sum of a sequence
 * P(a, b, c) = ab + ac + bc
 * We need to find elements e1..en such that
 * P(E, e1, .., en) == 0
 * After some transformation with pen&paper we find out that only two elements suffices
 * 
 * P(E, e1, e2) = P(E, e1) + e2 * S(E, e1)
 * Let e1 == 1 - S(E), then e2 = -P(E, [1 - S(E)])
 */
static void solve() {
    int N, K;
    std::cin >> N >> K;
    std::vector<int64_t> data(N);
    for (auto& d : data)
        std::cin >> d;

    auto sum = std::accumulate(data.begin(), data.end(), 0);
    int64_t pairs{0};
    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j)
            pairs += data[i] * data[j];

    if (std::all_of(data.begin(), data.end(), [](int d){ return d == 0; })) {
        std::cout << '0';
        return;
    }

    if (K == 1) {
        if (sum == 0 || pairs % sum != 0) 
            std::cout << "IMPOSSIBLE";
        else
            std::cout << -pairs / sum;
    } else {
        const int64_t e1 = 1 - sum; // sum**2 can't overflow, must be int64_t
        const int64_t e2 = pairs + e1 * sum;
        std::cout << e1 << ' ' << -e2;
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
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 squary.cpp -o squary.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address squary.cpp -o squary

Run:
squary.exe < squary.in

Input:

7
2 1
-2 6
2 1
-10 10
1 1
0
3 1
2 -2 2
3 10
-2 3 6
6 2
-2 2 1 -2 4 -1
1 12
-5

Output:

Case #1: 3
Case #2: IMPOSSIBLE
Case #3: 0
Case #4: 2
Case #5: -6 42
Case #6: -1 15
Case #7: 6 30

*/

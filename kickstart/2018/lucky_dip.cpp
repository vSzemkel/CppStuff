
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

// Lucky Dip
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050edf/0000000000050e1d

static void solve() {
    int N, K;
    std::cin >> N >> K;
    std::vector<double> dips(N);
    for (auto& d : dips)
        std::cin >> d;

    std::sort(dips.begin(), dips.end());
    std::vector<double> prefsum(N + 1);
    std::partial_sum(dips.begin(), dips.end(), prefsum.begin() + 1);
    const auto tail = [&prefsum](const int i){ return prefsum.back() - prefsum[i]; };

    double ans = double(prefsum[N]) / N;
    while (K--) {
        const auto it = std::lower_bound(dips.begin(), dips.end(), ans);
        const auto head_count = it - dips.begin();
        const auto tail_sum = tail(head_count);
        ans = (ans * head_count + tail_sum) / N;
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
        std::cout << "Case #" << g << ": " << std::fixed << std::setprecision(9); solve(); std::cout << '\n';
    }
}

/*

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++17 lucky_dip.cpp -o lucky_dip.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address lucky_dip.cpp -o lucky_dip

Run:
lucky_dip.exe < lucky_dip.in

Input:

5
4 0
1 2 3 4
3 1
1 10 1
3 15
80000 80000 80000
1 1
10
5 3
16 11 7 4 1

Output:

Case #1: 2.500000000
Case #2: 6.000000000
Case #3: 80000.000000000
Case #4: 10.000000000
Case #5: 12.358400000

*/

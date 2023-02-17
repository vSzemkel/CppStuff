
#include <algorithm>
#include <iostream>
#include <vector>

// The Equation
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050e02/000000000018fe36

static void solve() {
    int N;
    int64_t M;
    std::cin >> N >> M;
    std::vector<int64_t> data(N);
    int64_t max_bit{0};
    for (auto& d : data) {
        std::cin >> d;
        while (d > (int64_t{1} << max_bit))
            ++max_bit;
    }

    if (std::all_of(data.begin(), data.end(), [](auto d){ return d == 0; })) {
        std::cout << M / N;
        return;
    }

    const int64_t MAX = (int64_t{1} << max_bit); // Ai <= MAX
    std::vector<int> stats(max_bit + 1);
    std::vector<int64_t> obligatory_minimum(1);
    for (int64_t mask = 1, bit = 0; bit <= max_bit; mask <<= 1, ++bit) {
        for (const auto d : data)
            if (d & mask)
                ++stats[bit];
        obligatory_minimum.push_back(obligatory_minimum.back() + mask * std::min(N - stats[bit], stats[bit]));
    }

    int64_t ans{}, sum{};
    for (auto p = MAX, bit = max_bit; p; p >>= 1, --bit) {
        const auto base = sum + obligatory_minimum[bit];
        if (base + (N - stats[bit]) * p <= M) {
            sum += (N - stats[bit]) * p;
            ans += p;
        } else if (base + stats[bit] * p <= M) {
            sum += stats[bit] * p;
        } else {
            std::cout << -1;
            return;
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
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++17 the_equation.cpp -o the_equation.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address the_equation.cpp -o the_equation

Run:
the_equation.exe < the_equation.in

Input:

4
3 27
8 2 4
4 45
30 0 4 11
1 0
100
6 2
5 5 1 5 1 0

Output:

Case #1: 12
Case #2: 14
Case #3: 100
Case #4: -1 

*/


#include <cmath>
#include <iostream>

// Incremental House Of Pancakes
// https://codingcompetitions.withgoogle.com/codejam/round/000000000019ffb9/00000000003384ea



constexpr int64_t natural_sum(const int64_t n) {
    return n * (n + 1) / 2;
}

void solve() {
    int64_t L, R; std::cin >> L >> R;
    int64_t served, left, right;
    const auto sum = L + R;

    if (sum <= 3) {
        left = L;
        right = R;
        int to_serve = 1;
        while (true) {
            if (left >= right) {
                if (left >= to_serve)
                    left -= to_serve;
                else
                    break;
            } else if (right >= to_serve)
                right -= to_serve;
            else
                break;
            ++to_serve;
        }
        served = to_serve - 1;
    } else {
        bool swapped = L < R;
        if (swapped) std::swap(L, R);
        const auto diff = L - R;

        int64_t prolog = std::floor(std::sqrt(2 * diff));
        while (natural_sum(prolog) <= diff) ++prolog;
        --prolog;
        const auto sold_in_prolog = natural_sum(prolog);
        const auto rem = diff - sold_in_prolog;
        if (rem == 0) swapped = false;

        served = std::floor(std::sqrt(2 * sum));
        while (natural_sum(served) <= sum) ++served;
        --served;

        int64_t epilog = served - prolog;
retry:
        left = rem + epilog / 2;
        if (epilog % 2 == 1)
            left -= served;

        const auto not_sold = sum - natural_sum(served);
        if (std::abs(left) <= not_sold) {
            left = (left + not_sold) / 2;
            right = not_sold - left;
        } else {
            --served;
            --epilog;
            goto retry;
        }

        if (swapped) std::swap(left, right);
    }


    std::cout << served << " " << left << " " << right << "\n";
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve();
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 incremental_house_of_pancakes.cpp -o incremental_house_of_pancakes.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 incremental_house_of_pancakes.cpp -o incremental_house_of_pancakes.o

Run:
py.exe interactive_runner.py py.exe incremental_house_of_pancakes_testing_tool.py 1 -- incremental_house_of_pancakes.exe
incremental_house_of_pancakes.exe < incremental_house_of_pancakes.in

Input:

3
1 2
2 2
8 11

Output:

Case #1: 1 1 1
Case #2: 2 1 0
Case #3: 5 0 4
Case #4: 5 4 1
Case #5: 6 0 0
Case #6: 195 142 59

*/

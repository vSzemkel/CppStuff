
#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

// Unit Time Scheduling
// Example of matroid problem, greedy solution - schedule tasks minimizing penalty for excessing dealines

constexpr const int N = 7;

static void solve(const std::array<std::array<int, 2>, N>& deadlines_wages) {
    std::array<int, N> ans{};
    std::vector<bool> good(N);
    int cnt{1}, min_penalty{N}, total_penalty{0}, first_free{0}, last_free = N - 1;
    std::cout << "\nInput data (deadline, penalty): ";
    for (const auto [w, d] : deadlines_wages) {
        std::cout << '(' << d << ", " << w << ") ";
        int pos = d - 1;
        while (first_free <= pos && ans[pos]) --pos;
        if (first_free <= pos) {
            ans[pos] = cnt;
            good[pos] = true;
            while (first_free < N && ans[first_free]) ++first_free;
        } else {
            total_penalty += w;
            ans[last_free] = cnt;
            min_penalty = last_free;
            while (last_free >= 0 && ans[last_free]) --last_free;
        }
        ++cnt;
    }

    // only for pretty printing
    int lo{0}, hi{N - 1};
    while (lo < hi) {
        if (!good[lo] && good[hi]) {
            std::swap(ans[lo], ans[hi]);
            ++min_penalty;
            ++lo; --hi;
        }
        if (good[lo])
            ++lo;
        if (!good[hi])
            --hi;
    }

    cnt = 0;
    std::cout << "\nTotal penalty is: " << total_penalty << " with sequence: ";
    for (const int i : ans) {
        if (cnt == min_penalty) 
            std::cout << "|| ";
        std::cout << '(' << deadlines_wages[i - 1][1] << ", " << deadlines_wages[i - 1][0] << ") ";
        ++cnt;
    }
}

int main(int, char**)
{
    std::array<std::array<int, 2>, N> deadlines_wages = {
        std::array{70, 4}, // { wage, deadline }
        {60, 2},
        {50, 4},
        {40, 3},
        {30, 1},
        {20, 4},
        {10, 6}
    };

    std::sort(deadlines_wages.begin(), deadlines_wages.end(), std::greater<>{});
    solve(deadlines_wages);
    for (auto& dw : deadlines_wages)
        dw[2] = 90 - dw[2];
    std::sort(deadlines_wages.begin(), deadlines_wages.end(), std::greater<>{});
    solve(deadlines_wages);
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 unit_scheduling.cpp -o unit_scheduling.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address unit_scheduling.cpp -o unit_scheduling

Output:

Input data (deadline, penalty): (4, 70) (2, 60) (4, 50) (3, 40) (1, 30) (4, 20) (6, 10) 
Total penalty is: 50 with sequence: (3, 40) (2, 60) (4, 50) (4, 70) (6, 10) || (4, 20) (1, 30) 
Input data (deadline, penalty): (6, 70) (4, 70) (4, 60) (1, 50) (3, 40) (4, 30) (2, 20)
Total penalty is: 50 with sequence: (1, 50) (3, 40) (4, 60) (4, 70) (6, 70) || (2, 20) (4, 30)

*/

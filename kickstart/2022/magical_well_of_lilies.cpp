
#include <iostream>

// Magical Well Of Lilies
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb1b6/0000000000c47e79

int L, ans;

static void inner_solve(int acc, int mem, int cost) { // O(3^N)
    if (acc >= L) {
        if (acc == L && cost < ans)
            ans = cost;
        return;
    }

    if (cost + 1 < ans)
        inner_solve(acc + 1, mem, cost + 1);
    if (0 < mem && cost + 2 < ans)
        inner_solve(acc + mem, mem, cost + 2);
    if (mem < acc && cost + 4 < ans)
        inner_solve(acc, acc, cost + 4);
}

static void solve() {
    std::cin >> L;

    ans = 1e09;
    inner_solve(0, 0, 0);
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 magical_well_of_lilies.cpp -o magical_well_of_lilies.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address magical_well_of_lilies.cpp -o magical_well_of_lilies

Run:
magical_well_of_lilies.exe < magical_well_of_lilies.in

Input:

3
17
18
19

Output:

Case #1: 15
Case #2: 14
Case #3: 15

*/


#include <iostream>
#include <numeric>
#include <vector>

// Sample problem
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008f4332/0000000000942404

static void solve() {
    int N, M;
    std::cin >> N >> M;
    std::vector<int> bags(N);
    for (auto& b : bags)
        std::cin >> b;

    const int total = std::accumulate(bags.begin(), bags.end(), 0);
    std::cout << total % M;
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 sample_problem.cpp -o sample_problem.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address sample_problem.cpp -o sample_problem

Run:
sample_problem.exe < sample_problem.in

Input:

2
7 3
1 2 3 4 5 6 7
5 10
7 7 7 7 7

Output:

Case #1: 1
Case #2: 5

*/

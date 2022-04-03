
#include <algorithm>
#include <iostream>
#include <vector>

// d1000000
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000876ff1/0000000000a46471

static void solve() {
    int N; std::cin >> N;
    std::vector<int> dies(N);
    for (auto& d : dies)
        std::cin >> d;
    std::sort(dies.begin(), dies.end());

    int count{0};
    for (const int d : dies)
        if (count < d)
            ++count;

    std::cout << count;
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
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 d1000000.cpp -o d1000000.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address d1000000.cpp -o d1000000

Run:
d1000000.exe < d1000000.in

Input:

4
4
6 10 12 8
6
5 4 5 4 4 4
10
10 10 7 6 7 4 4 5 7 4
1
10

Output:


*/

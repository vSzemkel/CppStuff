
#include <iostream>
#include <vector>

// Countdown
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ff43/00000000003380d2


static void solve() {
    int N, K; std::cin >> N >> K;
    std::vector<int> data(N);
    for (auto& n : data)
        std::cin >> n;

    int ret{0}, pos{0};
    while (true) {
        while (pos < N && data[pos] != K) ++pos;
        if (pos == N) break;
        int count{K};
        while (pos < N && count > 0 && data[pos] == count) {
            ++pos;
            --count;
        }
        if (count == 0)
            ++ret;
    }
    
    std::cout << ret;
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 countdown.cpp -o countdown.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address countdown.cpp -o countdown

Run:
countdown.exe < countdown.in

Input:

3
12 3
1 2 3 7 9 3 2 1 8 3 2 1
4 2
101 100 99 98
9 6
100 7 6 5 4 3 2 1 100

Output:

Case #1: 2
Case #2: 0
Case #3: 1

*/

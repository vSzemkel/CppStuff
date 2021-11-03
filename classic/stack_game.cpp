
#include <iostream>
#include <vector>

// Stack game
// In this game one can pop 1 or stack.top() elements from the stack.
// Empty stack is loosing. Is there a winning strategy for the starting player?


static void solve() {
    int N; std::cin >> N;
    std::vector<int> game(N);
    for (auto& v : game)
        std::cin >> v;

    std::vector<bool> dp(N + 1); // dp[K] = result for the stack with first K elements from game
    dp[1] = true; // single element always wins
    for (int i = 2; i <= N; ++i) {
        const int elem = game[i - 1];
        dp[i] = !dp[i - 1];
        if (!dp[i] && 1 < elem && elem <= i)
            dp[i] = !dp[i - elem];
    }

    std::cout << (dp.back() ? "YES" : "NO");
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 stack_game.cpp -o stack_game.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address stack_game.cpp -o stack_game

Run:
stack_game.exe < stack_game.in

Input:

4
2
1 2
2
1 1
3
1 1 1
3
1 2 2

Output:

Case #1: YES
Case #2: NO
Case #3: YES
Case #4: NO

*/

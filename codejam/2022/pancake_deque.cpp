
#include <iostream>
#include <vector>

// Pancake Deque
// https://codingcompetitions.withgoogle.com/codejam/round/000000000087711b/0000000000acd59d

static void solve() {
    int N; std::cin >> N;
    std::vector<int> pancakes(N);
    for (auto& p : pancakes)
        std::cin >> p;
 
    int ans{0}, lb{0}, rb{N - 1}, prev{-1}, next;
    while (lb <= rb) {
        next = pancakes[lb] < pancakes[rb] ? pancakes[lb++] : pancakes[rb--];
        if (prev <= next) {
            prev = next;
            ++ans;
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
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 pancake_deque.cpp -o pancake_deque.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address pancake_deque.cpp -o pancake_deque

Run:
pancake_deque.exe < pancake_deque.in

Input:

5
2
1 5
4
1 4 2 3
5
10 10 10 10 10
4
7 1 3 1000000
5
100 200 150 50 100

Output:

Case #1: 2
Case #2: 3
Case #3: 5
Case #4: 2
Case #5: 4

*/

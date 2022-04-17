
#include <algorithm>
#include <iostream>
#include <vector>

// Training
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050e01/00000000000698d6

static void solve() {
    int N, P;
    std::cin >> N >> P;
    std::vector<int> students(N);
    for (auto& s : students)
        std::cin >> s;

    std::sort(students.begin(), students.end());
    int64_t ans{0}, target = students[P - 1];
    for (int i = 0; i < P - 1; ++i)
        ans += target - students[i];
    int64_t can = ans;
    for (int i = P; i < N; ++i) {
        can = can - (students[i - 1] - students[i - P]) + (P - 1) * (students[i] - students[i - 1]);
        ans = std::min(ans, can);
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
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 training.cpp -o training.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address training.cpp -o training

Run:
training.exe < training.in

Input:

3
4 3
3 1 9 100
6 2
5 5 1 2 3 4
5 5
7 7 1 7 7

Output:

Case #1: 14
Case #2: 0
Case #3: 6

*/

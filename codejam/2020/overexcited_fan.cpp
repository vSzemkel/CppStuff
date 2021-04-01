
#include <iostream>
#include <string>

// Overexcited Fan
// https://codingcompetitions.withgoogle.com/codejam/round/000000000019fef4/0000000000317409

void solve() {
    int X, Y; 
    std::string path;
    std::cin >> X >> Y >> path;

    if (X == 0 && Y == 0) {
        std::cout << "0\n";
        return;
    }

    int timer{0};
    for (const char d : path) {
        switch (d) {
            case 'N': ++Y; break;
            case 'S': --Y; break;
            case 'E': ++X; break;
            case 'W': --X; break;
        }
        ++timer;
        if (abs(X) + abs(Y) <= timer) {
            std::cout << timer << "\n";
            return;
        }
    }

    std::cout << "IMPOSSIBLE\n";
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
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 overexcited_fan.cpp -o overexcited_fan.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 overexcited_fan.cpp -o overexcited_fan.o

Run:
py.exe interactive_runner.py py.exe overexcited_fan_testing_tool.py 1 -- overexcited_fan.exe
overexcited_fan.exe < overexcited_fan.in

Input:

7
4 4 SSSS
3 0 SNSS
2 10 NSNNSN
0 1 S
2 7 SSSSSSSS
3 2 SSSW
4 0 NESW

Output:

Case #1: 4
Case #2: IMPOSSIBLE
Case #3: IMPOSSIBLE
Case #4: 1
Case #5: 5
Case #6: 4
Case #7: 4

*/

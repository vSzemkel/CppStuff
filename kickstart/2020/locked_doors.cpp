
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <utility>
#include <vector>

// Locked Doors
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ff08/0000000000386d5c

static void solve_set1() {
    int N, Q; std::cin >> N >> Q;
    std::vector<int> doors(N - 1);
    for (auto& d : doors)
        std::cin >> d;
    doors.push_back(std::numeric_limits<int>::max());

    for (int q = 0; q < Q; ++q) {
        int S, K; std::cin >> S >> K; --S;
        int ans{S};
        int l = S - 1;
        int r = S;
        while (--K > 0) {
            if (l >= 0 && doors[l] < doors[r]) {
                ans = l;
                --l;
            } else {
                ++r;
                ans = r;
            }
        }

        std::cout << ans + 1 << ' ';
    }
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve_set1(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 locked_doors.cpp -o locked_doors.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address locked_doors.cpp -o locked_doors

Run:
locked_doors.exe < locked_doors.in

Input:

2
5 4
90 30 40 60
3 4
3 1
1 5
4 3
10 2
6 2 4 5 9 30 7 1 8
6 8
6 8

Output:


*/

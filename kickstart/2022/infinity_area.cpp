
#include <iomanip>
#include <iostream>

// Infinity Area
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008caa74/0000000000acf079

constexpr const double PI = 3.141592653589793238462643;

static void solve() {
    int64_t A, B, R;
    std::cin >> R >> A >> B;

    double ans{0};
    bool draw_right{true};
    while (R) {
        ans += PI * R * R;
        if (draw_right)
            R *= A;
        else
            R /= B;
        draw_right = !draw_right;
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
        std::cout << "Case #" << g << ": " << std::fixed << std::setprecision(9); solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 infinity_area.cpp -o infinity_area.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address infinity_area.cpp -o infinity_area

Run:
infinity_area.exe < infinity_area.in

Input:

2
1 3 6
5 2 5

Output:

Case #1: 31.415926536
Case #2: 455.530934771

*/

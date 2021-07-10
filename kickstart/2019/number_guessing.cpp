
#include <iostream>
#include <string>

// Number Guessing
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000051060/00000000000588f4


static void solve() {
    int A, B, N; std::cin >> A >> B >> N;
    ++A; ++B;
    while (A <= B) {
        const int guess = A + (B - A) / 2;
        std::cout << guess << std::endl;
        std::string answer; std::cin >> answer;
        if (answer == "CORRECT")
            return;
        if (answer == "TOO_BIG")
            B = guess - 1;
        else // answer == "TOO_SMALL"
            A = guess + 1;
    }
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) 
        solve();
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 number_guessing.cpp -o number_guessing.exe

*/

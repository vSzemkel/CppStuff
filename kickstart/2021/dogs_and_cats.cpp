
#include <iostream>
#include <string>

// Dogs and cats
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000004362d6/00000000008b3771


static void solve() {
    int64_t N, D, C, M;
    std::cin >> N >> D >> C >> M;
    std::string line; std::cin >> line;

    const auto ld = line.rfind('D');
    if (ld == std::string::npos) {
        std::cout << "YES";
        return;
    } else
        line.resize(ld + 1);

    bool ok{true};
    for (const int a : line) {
        if (a == 'C') {
            if (C > 0) --C;
            else ok = false;
        } else {
            if (D > 0) {
                --D;
                C += M;
            } else ok = false;
        }
        if (!ok) break;
    }

    std::cout << (ok ? "YES" : "NO");
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 dogs_and_cats.cpp -o dogs_and_cats.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address dogs_and_cats.cpp -o dogs_and_cats

Run:
dogs_and_cats.exe < dogs_and_cats.in

Input:

3
6 10 4 0
CCDCDD
4 1 2 0
CCCC
4 2 1 0
DCCD

Output:

Case #1: YES
Case #2: YES
Case #3: NO

*/

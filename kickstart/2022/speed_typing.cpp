
#include <iostream>
#include <string>

// Speed Typing
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb33e/00000000009e7021


static void solve() {
    std::string I, P;
    std::cin >> I >> P;

    int ret{0}, pi{0}, pp{0};
    const int si = int(I.size()), sp = int(P.size());
    while (pi < si && pp < sp) {
        while (pp < sp && I[pi] != P[pp]) {
            ++ret;
            ++pp;
        }
        ++pi;
        ++pp;
    }

    if (pi < si || pp > sp)
        std::cout << "IMPOSSIBLE";
    else {
        if (pi == si)
            ret += sp - pp;
        std::cout << ret;
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
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 speed_typing.cpp -o speed_typing.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address speed_typing.cpp -o speed_typing

Run:
speed_typing.exe < speed_typing.in

Input:

2
Ilovecoding
IIllovecoding
KickstartIsFun
kkickstartiisfun

Output:

Case #1: 2
Case #2: IMPOSSIBLE

*/

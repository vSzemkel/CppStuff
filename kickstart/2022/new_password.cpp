
#include <iostream>
#include <string>

// New Password
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb4d1/0000000000b20f15


static void solve() {
    int N;
    std::string S;
    std::cin >> N >> S;

    bool low{}, up{}, dig{}, sp{};
    for (const char c : S) {
        if ('a' <= c && c <= 'z') {
            low = true;
            continue;
        }
        if ('A' <= c && c <= 'Z') {
            up = true;
            continue;
        }
        if ('0' <= c && c <= '9') {
            dig = true;
            continue;
        }
        sp = true;
    }

    if (!low)
        S += 'a';
    if (!up)
        S += 'A';
    if (!dig)
        S += '0';
    if (!sp)
        S += '&';
    while (S.size() < 7)
        S += 'a';

    std::cout << S;
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
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 new_password.cpp -o new_password.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address new_password.cpp -o new_password

Run:
new_password.exe < new_password.in

Input:

5
7
1234567
10
1111234567
1
A
2
1*
7
1234aB&

Output:

Case #1: 1234567aA&
Case #2: 1111234567aA&
Case #3: Aa0&aaa
Case #4: 1*aAaaa
Case #5: 1234aB&

*/


#include <algorithm>
#include <iostream>
#include <string>

// Centauri prime
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008f4332/0000000000941ec5

const std::string vowels = "aeiou";

static void solve() {
    std::string K, R;
    std::cin >> K;

    const char e = K.back() | 0x20;
    if (e == 'y')
        R = "nobody";
    else if (std::binary_search(vowels.begin(), vowels.end(), e))
        R = "Alice";
    else
        R = "Bob";

    std::cout << K << " is ruled by " << R << '.';
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 centauri_prime.cpp -o centauri_prime.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address centauri_prime.cpp -o centauri_prime

Run:
centauri_prime.exe < centauri_prime.in

Input:

3
Mollaristan
Auritania
Zizily

Output:

Case #1: Mollaristan is ruled by Bob.
Case #2: Auritania is ruled by Alice.
Case #3: Zizily is ruled by nobody.

*/

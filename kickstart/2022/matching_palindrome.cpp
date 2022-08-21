
#include <iostream>
#include <string>
#include <string_view>

// Matching Palindrome
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb0f5/0000000000ba82c5

/**
 * @brief Observation: if S is composed of N equal chunks, then adding one chunk is a solution
 * As chunks[0] == chunks[N-1] == reverse(chunks[N-1]) we know that chunk is a palindrome
 */
static void solve() {
    int N;
    std::string S;
    std::cin >> N >> S;

    for (int i = 1; i <= N; ++i) {
        if (N % i)
            continue;

        bool ok{true};
        const int z = N / i - 1;
        const auto chunk = std::string_view(S.data(), i);
        for (int j = 1; j <= z; ++j)
            if (std::string_view(S.data() + i * j, i) != chunk) {
                ok = false;
                break;
            }

        if (ok) {
            std::cout << chunk;
            return;
        }
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 matching_palindrome.cpp -o matching_palindrome.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address matching_palindrome.cpp -o matching_palindrome

Run:
matching_palindrome.exe < matching_palindrome.in

Input:

3
4
abba
4
cccc
6
cdccdc

Output:

Case #1: abba
Case #2: c
Case #3: cdc

*/

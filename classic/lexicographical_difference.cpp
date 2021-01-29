
#include <algorithm>
#include <iostream>
#include <vector>

// Lexicographical difference
// https://en.wikipedia.org/wiki/Levenshtein_distance

int levenstine(const std::string& s1, const std::string& s2)
{
    const int size1 = s1.size();
    const int size2 = s2.size();
    std::vector<std::vector<int>> cost(size1 + 1, std::vector<int>(size2 + 1));
    for (int r = 1; r <= size1; ++r) cost[r][0] = r;
    for (int c = 1; c <= size2; ++c) cost[0][c] = c;
    for (int r = 1; r <= size1; ++r)
        for (int c = 1; c <= size2; ++c) {
            const int substitution_cost = (s1[r - 1] == s2[c - 1] ? 0 : 1);
            cost[r][c] = std::min(std::min(
                cost[r - 1][c] + 1,
                cost[r][c - 1] + 1),
                cost[r - 1][c - 1] + substitution_cost);
        }

    return cost[size1][size2];
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::string s1, s2; std::cin >> s1 >> s2;
        std::cout << "Case #" << g << ": " <<  levenstine(s1, s2) << "\n";
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 lexicographical_difference.cpp -o lexicographical_difference.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 lexicographical_difference.cpp -o lexicographical_difference.o

Run:
lexicographical_difference.exe < lexicographical_difference.in

Input:

5
AABC CABA
ABCDEF AEBC
MARCIN MARIAN
SUNDAY SATURDAY
ABCDEFABDDEFABCDEFABCAEFABCDEF ADCDEFABCDEFABBDEFABCDEFABCDEA

Output:

Case #1: 2
Case #2: 4
Case #3: 2
Case #4: 3
Case #5: 5

*/

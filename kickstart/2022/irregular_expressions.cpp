
#include <cstring>
#include <iostream>
#include <unordered_set>
#include <vector>

// Irregular Expressions
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008f4a94/0000000000b55464

const std::unordered_set<char> v = {'a', 'e', 'i', 'o', 'u'};

static void solve() {
    std::string s;
    std::cin >> s;

    int sz{0};
    bool found{};
    std::vector<int> vowpos;
    std::vector<char> vowels;
    for (int i = 0; i < int(s.size()) && !found; ++i)
        if (v.count(s[i])) {
            ++sz;
            vowpos.push_back(i);
            vowels.push_back(s[i]);
            if (sz >= 5) {
                const int first = vowels[sz - 2], second = vowels.back(), 
                          last = sz - 5, rsz = vowpos[sz - 1] - vowpos[sz - 2];
                int cur{0};
                while (cur <= last) {
                    if (vowels[cur] == first && vowels[cur + 1] == second) {
                        const int csz = vowpos[cur + 1] - vowpos[cur];
                        if (csz == rsz && !memcmp(s.data() + vowpos[cur], s.data() + vowpos[sz - 2], csz)) {
                            found = true;
                            break;
                        }
                    }
                    ++cur;
                }
            }
        }

    std::cout << (found ? "Spell!" : "Nothing.");
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 irregular_expressions.cpp -o irregular_expressions.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address irregular_expressions.cpp -o irregular_expressions

Run:
py.exe interactive_runner.py py.exe irregular_expressions_testing_tool.py 1 -- irregular_expressions.exe
irregular_expressions.exe < irregular_expressions.in

Input:

4
abracadabra
kajabbamajabbajab
frufrumfuffle
schprexityschprex

Output:

Case #1: Spell!
Case #2: Spell!
Case #3: Nothing.
Case #4: Nothing.

*/

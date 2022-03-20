
#include <iostream>
#include <string>

// Challenge Nine
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb33e/00000000009e7997

static void solve() {
    std::string s;
    std::cin >> s;
    int sum{0};
    for (const char c : s)
        sum += c - '0';
    char newone = '0' + 9 - (sum % 9);
    if (newone == '9')
        newone = '0';

    const int sz = int(s.size());
    int pos = newone == '0' ? 1 : 0;
    std::string r = s;
    r.push_back(newone);
    while (pos < sz) {
        while (pos < sz && s[pos] < newone)
            ++pos;
        auto c = s;
        c.insert(c.begin() + pos, newone);
        r = std::min(r, c);
        ++pos;
    }

    std::cout << r;
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 challenge_nine.cpp -o challenge_nine.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address challenge_nine.cpp -o challenge_nine

Run:
challenge_nine.exe < challenge_nine.in

Input:

3
5
33
12121

Output:

Case #1: 45
Case #2: 333
Case #3: 121212

*/

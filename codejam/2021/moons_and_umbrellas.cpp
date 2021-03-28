
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

static void ecnerwala()
{
    int X, Y; std::string text; std::cin >> X >> Y >> text;

    const int INF = 1e9;
    std::vector<int> dp(2);
    for (int i = 0; i < int(text.size()); i++) {
        if (i) {
            dp = {std::min(dp[0], dp[1] + Y), std::min(dp[1], dp[0] + X)};
        }
        char c = text[i];
        if (c == 'C')
            dp[1] = INF;
        else if (c == 'J')
            dp[0] = INF;
    }

    std::cout << std::min(dp[0], dp[1]);
}

static void smart()
{
    int X, Y; std::string text; std::cin >> X >> Y >> text;

    std::string cleaned;
    std::copy_if(text.begin(), text.end(), std::back_inserter(cleaned), [](const char c){
        return c != '?';
    });

    int cost{0};
    for (int i = 0; i < (int)cleaned.size() - 1; ++i) {
        if (cleaned[i] == 'C' && cleaned[i + 1] == 'J') cost += X;
        if (cleaned[i] == 'J' && cleaned[i + 1] == 'C') cost += Y;
    }

    std::cout << cost;
}

// CJ cost X
// JC cost Y
static void solve()
{
    int X, Y; std::string text; std::cin >> X >> Y >> text;
    int size = (int)text.size();

    int lpos{0};
    while (lpos < size && text[lpos] == '?') ++lpos;
    int rpos{size - 1};
    while (rpos >=0 && text[rpos] == '?') --rpos;
    if (rpos <= lpos) {
        std::cout << 0;
        return;
    }

    size = rpos - lpos + 1;
    text = text.substr(lpos, size);

    int pos{0}, cost{0};
    char prev = text[0];
    while (pos < size) {
        while (pos < size && text[pos] == prev) ++pos;
        if (pos == size) break;
        char cur = text[pos];
        if (cur == '?') {
            while (text[pos] == '?') ++pos;
            cur = text[pos];
        }
        if (prev == 'C' && cur == 'J') cost += X;
        if (prev == 'J' && cur == 'C') cost += Y;
        prev = cur;
    }

    std::cout << cost;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; ecnerwala(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 moons_and_umbrellas.cpp -o moons_and_umbrellas.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address moons_and_umbrellas.cpp -o moons_and_umbrellas.o

Run:
moons_and_umbrellas.exe < moons_and_umbrellas.in

Input:

5
2 3 CJ?CC?
4 2 CJCJ
1 3 C?J
2 5 ??J???
5 7 ?????????CCC?JJ????C?????

Output:

Case #1: 5
Case #2: 10
Case #3: 1
Case #4: 0
Case #5: 12

*/

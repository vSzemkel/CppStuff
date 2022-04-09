
#include <iostream>
#include <string>

// Double or One Thing
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000877ba5/0000000000aa8e9c

static void solve() {
    std::string R, S;
    std::cin >> S;

    const int len = int(S.size());
    for (int i = 0; i < len; ) {
        int j = i++;
        while (i < len && S[j] == S[i])
            ++i;
        const int f = (i < len && S[j] < S[i]) ? 2 : 1;
        for (int z = f * (i - j); z; --z)
            R += S[j];
    }

    std::cout << R;
}

static void submitted() {
    std::string S;
    std::cin >> S;

    std::string R;
    const int len = int(S.size());
    for (int i = 1; i < len; ++i) {
        R += S[i - 1];
        if (S[i - 1] < S[i])
            R += S[i - 1];
        if (S[i - 1] == S[i]) {
            int j{i};
            while (j < len && S[j - 1] == S[j]) ++j;
            if (j < len && S[i] < S[j]) {
                for (int z = 2*(j - i) + 1; z; --z)
                    R += S[i - 1];
                i = j;
            }
        }
    }
    R += S.back();

    for (int i = 0; i < len; ++i)
        if (R[i] != S[i]) {
            std::cout << R;
            return;
        }

    std::cout << S;
}

static void ksun48() {
    std::string ans, s; std::cin >> s;
    for (auto it = s.rbegin(); it != s.rend(); ++it) {
        ans = *it + ans;
        ans = std::min(ans, *it + ans);
    }
    std::cout << ans;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; ksun48(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 double_or_one_thing.cpp -o double_or_one_thing.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address double_or_one_thing.cpp -o double_or_one_thing

Run:
double_or_one_thing.exe < double_or_one_thing.in

Input:

5
PEEL
ABBBA
ABBBAC
AAAAAAAAAA
CODEJAMDAY

Output:

Case #1: PEEEEL
Case #2: AABBBA
Case #3: AABBBAAC
Case #4: AAAAAAAAAA
Case #5: CCODDEEJAAMDAAY

*/

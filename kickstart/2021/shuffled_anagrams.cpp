
#include <algorithm>
#include <array>
#include <assert.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// Shuffled anagrams
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000043585c/000000000085a152


static void solve_smart() { // by Aeren
    std::string S; std::cin >> S;
    const int size = int(S.size());
    std::array<std::vector<int>, 26> pos;
    std::array<int, 26> cnt{};
    for (int i = 0; i < size; ++i) {
        const int letter = S[i] - 'a';
        ++cnt[letter];
        pos[letter].push_back(i);
    }

    if (*std::max_element(cnt.begin(), cnt.end()) > size / 2) {
        std::cout << "IMPOSSIBLE";
        return;
    }

    // sorted letters positions
    std::vector<int> srtind;
    for (const auto& lp : pos)
        srtind.insert(srtind.end(), lp.begin(), lp.end());

    // letters in srtind[i] and srtind[(i + size / 2) % size] differs
    std::string ret(size, '-');
    for (int i = 0; i < size; ++i)
        ret[srtind[i]] = S[srtind[(i + size / 2) % size]];

    std::cout << ret;
}

static void solve() {
    std::string S; std::cin >> S;
    const int size = int(S.size());
    std::unordered_map<char, int> stat;
    for (const char c : S) ++stat[c];
    std::vector<std::pair<int, char>> ord;
    for (const auto& s : stat) ord.emplace_back(s.second, s.first);
    std::sort(ord.begin(), ord.end(), std::greater{});
    if (ord.front().first > size / 2) {
        std::cout << "IMPOSSIBLE";
        return;
    }

    std::string A(size, '-');
    for (const auto& [count, letter] : ord)
        for (int i = 0, j = 0; i < count; ++i)
            for (; j < size; ++j)
                if (A[j] == '-' && S[j] != letter) {
                    A[j++] = letter;
                    break;
                }

    size_t bad{S.size()};
    while (true) {
        bad = A.rfind('-', bad - 1);
        if (bad == std::string::npos) break;
        for (int i = 0; i < int(bad); ++i)
            if (A[i] != S[bad] && S[i] != S[bad]) {
                A[bad] = A[i];
                A[i] = S[bad];
                break;
            }
    }

    std::cout << A;
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 shuffled_anagrams.cpp -o shuffled_anagrams.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address shuffled_anagrams.cpp -o shuffled_anagrams

Run:
shuffled_anagrams.exe < shuffled_anagrams.in

Input:

3
aaaacccbbb
bbbaaaaccc
aaaabbbccc

Output:

Case #1: bbcbaaaacc
Case #2: aaacccbabb
Case #3: cccbaaaabb

*/

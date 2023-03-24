
#include <array>
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Scrambled Words
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050edf/0000000000051004

using freq_t = std::array<int, 26>;

static freq_t scan(std::string_view sv) {
    freq_t ret{};
    for (const char c : sv)
        ++ret[c - 'a'];
    return ret;
}

static void solve() {
    int L; std::cin >> L;
    std::vector<std::string> words(L);
    std::vector<freq_t> midwords;
    midwords.reserve(L);
    for (auto& w : words) {
        std::cin >> w;
        const auto m = w.substr(1, w.size() - 2);
        midwords.push_back(scan(m));
    }
    char c0, c1;
    int64_t N, A, B, C, D;
    std::cin >> c0 >> c1 >> N >> A >> B >> C >> D;
    std::string text = {c0, c1};
    std::unordered_map<char, std::unordered_set<int>> charpos;
    charpos[c0].insert(0);
    charpos[c1].insert(1);
    int n0{c0}, n1{c1};
    for (int i = 2; i < N; ++i) {
        const auto tmp = n1;
        n1 = (A * n1 + B * n0 + C) % D;
        n0 = tmp;
        const char next = 'a' + n1 % 26;
        text.push_back(next);
        charpos[next].insert(i);
    }


    int64_t ans{0};
    std::unordered_map<int, std::map<freq_t, bool>> cache;
    for (int i = 0; i < L; ++i) {
        const auto& w = words[i];
        const auto wf = w.front();
        const auto wb = w.back();
        const auto& m = midwords[i];
        const auto key = int(wf) * 27 + wb;
        const auto checked = cache[key].find(m);
        if (checked != cache[key].end()) {
            if (checked->second)
                ++ans;
            continue;
        }
        const auto& start = charpos[wf];
        const auto& end = charpos[wb];
        const auto msz = words[i].size() - 2;
        bool found{};
        for (const int s : start)
            if (end.count(s + msz + 1) > 0)
                if (scan(std::string_view{text.data() + s + 1, msz}) == m) {
                    found = true;
                    break;
                }
        cache[key][m] = found;
        if (found)
            ++ans;
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
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++17 scrambled_words.cpp -o scrambled_words.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address scrambled_words.cpp -o scrambled_words

Run:
scrambled_words.exe < scrambled_words.in

Input:

1
5
axpaj apxaj dnrbt pjxdn abd
a a 50 1 1 1 30

Output:

Case #1: 4

*/

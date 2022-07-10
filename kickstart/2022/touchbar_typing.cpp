
#include <algorithm>
#include <iostream>
#include <functional>
#include <map>
#include <unordered_map>
#include <utility>
#include <vector>

// Touchbar Typing
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008caea6/0000000000b76f44


static void solve() {
    int N; std::cin >> N;
    std::vector<int> S(N);
    for (auto& v : S)
        std::cin >> v;
    int M; std::cin >> M;
    std::vector<int> K(M);
    for (auto& v : K)
        std::cin >> v;

    std::unordered_map<int, std::vector<int>> pos; // positions of char on keyboard, {char -> positions in K}
    for (int i = 0; i < M; ++i)
        pos[K[i]].push_back(i);

    std::map<std::pair<int, int>, int64_t> cache;  // {pos_k, pos_s} -> ans
    std::function<int64_t(int, int)> solve_inner = [&](const int pos_k, const int pos_s) {
        if (pos_s == N)
            return int64_t{};

        auto& ce = cache[{pos_k, pos_s}];
        if (ce == 0) {
            const auto& curpos = pos[S[pos_s]];
            const auto lbit = std::lower_bound(curpos.begin(), curpos.end(), pos_k);
            if (lbit == curpos.begin())
                ce = std::abs(pos_k - curpos.front()) + solve_inner(curpos.front(), pos_s + 1);
            else if (lbit == curpos.end())
                ce = std::abs(pos_k - curpos.back()) + solve_inner(curpos.back(), pos_s + 1);
            else if (pos_k == *lbit)
                ce = solve_inner(pos_k, pos_s + 1);
            else // pos_k < *lbit
                ce = std::min(std::abs(pos_k - *lbit) + solve_inner(*lbit, pos_s + 1),
                              std::abs(pos_k - *std::prev(lbit)) + solve_inner(*std::prev(lbit), pos_s + 1));
        }

        return ce;
    };

    int64_t ans = 1e18;
    for (const int p : pos[S.front()])
        ans = std::min(ans, solve_inner(p, 1));

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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 touchbar_typing.cpp -o touchbar_typing.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address touchbar_typing.cpp -o touchbar_typing

Run:
touchbar_typing.exe < touchbar_typing.in

Input:

4
5
1 2 3 2 1
3
1 2 3
3
1 1 1
2
2 1
4
2 1 4 1
8
4 1 5 2 1 3 5 4
3
1 2 3
8
2 3 5 1 4 6 7 2

Output:

Case #1: 4
Case #2: 0
Case #3: 4
Case #4: 4

*/

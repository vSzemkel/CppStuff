/*
ID: marcin.8
LANG: C++
TASK: bigbrn
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=FWfFlfCOnfs&S=bigbrn
*/

#include <algorithm>
#include <fstream>
#include <functional>
#include <vector>

std::ifstream task_in("bigbrn.in");
std::ofstream task_out("bigbrn.out");

void solution1()
{
    int N, T, r, c;
    task_in >> N >> T;
    int ans{};
    if (T < N * N) {
        std::vector<bool> trees(N * N);
        std::vector<short> E(N * N, 1);
        auto S = E;

        for (; T; --T) {
            task_in >> r >> c;
            const auto pos = (r - 1) * N + c - 1;
            trees[pos] = true;
            E[pos] = S[pos] = 0;
        }

        // initialize walls
        for (r = N - 1; ~r; --r)
            for (c = N - 1; ~c; --c)
                if (const auto pos = r * N + c; !trees[pos]) {
                    if (c < N - 1)
                        E[pos] = E[pos + 1] + 1;
                    if (r < N - 1) 
                        S[pos] = S[pos + N] + 1;
                }

        ++ans;
        for (r = 0; r < N - ans; ++r)
            for (c = 0; c < N - ans; ++c) {
                const auto pos = r * N + c;
                auto target = std::min(E[pos], S[pos]);
                if (ans < target) {
                    for (int s = r + 1; s < r + target && ans < target; ++s)
                        target = std::min(target, E[s * N + c]);
                    if (ans < target)
                        ans = target;
                }
            }
    }

    task_out << ans << '\n';
}

/**
 * This is recommended solution from USACO, it is slower then solution1
*/
void solution2()
{
    short N, T, r, c;
    task_in >> N >> T;
    short ans{};
    if (T < N * N) {
        std::vector<bool> trees(N * N);
        std::vector<short> dp(N * N, -1);

        for (; T; --T) {
            task_in >> r >> c;
            trees[(r - 1) * N + c - 1] = true;
        }

        const std::function<short(int, int)> max_barn = [&](const int r, const int c) -> short {
            const auto pos = r * N + c;
            if (r < 0 || c < 0 || N <= r || N <= c || trees[pos])
                return 0;

            if (dp[pos] >= 0)
                return dp[pos];

            return dp[pos] = 1 + std::min({max_barn(r + 1, c), max_barn(r, c + 1), max_barn(r + 1, c + 1)});
        };

        ++ans;
        for (r = N - 1; ~r; --r)
            for (c = N - 1; ~c; --c)
                if (ans < max_barn(r, c))
                    ans = dp[r * N + c];
    }

    task_out << ans << '\n';
}

int main(int, char**)
{
    solution1();
    // solution2();
}
/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 bigbrn.cpp -o bigbrn.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address bigbrn.cpp -o bigbrn

Run:
bigbrn.exe && type bigbrn.out

Input:

1000 20 
209 226 
316 816 
159 596 
878 877 
503 626 
792 283 
97 91 
997 287 
27 842 
693 672 
838 620 
166 514 
835 546 
875 83 
434 713 
469 642 
938 136 
458 96 
732 687 
325 586

Output:

489

*/

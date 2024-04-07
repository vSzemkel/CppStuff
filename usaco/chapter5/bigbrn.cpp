/*
ID: marcin.8
LANG: C++
TASK: bigbrn
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=FWfFlfCOnfs&S=bigbrn
*/

#include <algorithm>
#include <fstream>
#include <vector>

std::ifstream task_in("bigbrn.in");
std::ofstream task_out("bigbrn.out");

int main(int, char**)
{
    int N, T, r, c;
    std::string comet, group;
    task_in >> N >> T;
    int bbc{N}, bbC{}, bbr{N}, bbR{};
    std::vector<std::vector<bool>> trees(N, std::vector<bool>(N));
    for (int z = T; z; --z) {
        task_in >> r >> c;
        trees[--r][--c] = true;
        bbr = std::min(bbr, r);
        bbR = std::max(bbR, r);
        bbc = std::min(bbc, c);
        bbC = std::max(bbC, c);
    }

    bool found{};
    std::vector<std::vector<bool>> barns(N, std::vector<bool>(N, true));
    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c) {
            barns[r][c] = !trees[r][c];
            if (barns[r][c])
                found = true;
        }

    int ans = std::max({bbc, bbr, N - 1 - bbC, N - 1 - bbR});
    if (found && T > 1)
        for (ans = 1; ans < N; ++ans) {
            found = false;
            for (int r = 0; r < N - ans; ++r)
                for (int c = 0; c < N - ans; ++c)
                    if (barns[r][c] && barns[r + 1][c + 1] && !trees[r + ans][c] && !trees[r][c + ans])
                        found = true;
                    else
                        barns[r][c] = false;

            if (!found)
                break;
        }

    task_out << ans << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 bigbrn.cpp -o bigbrn.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address bigbrn.cpp -o bigbrn

Input:

8 3
2 2
2 6
6 3

Output:

5

*/


#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

// Retiling
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000435915/00000000007dc2de

static void solve_set1_incorrect() { // need maximum matching in bipartite graph for this
    int R, C, F, S; std::cin >> R >> C >> F >> S;
    std::vector<std::string> start(R), final(R);
    for (auto& r : start)
        std::cin >> r;
    for (auto& r : final)
        std::cin >> r;

    int flips{0}, swapsh{0}, swapsv{0};
    std::set<int> tailsh, tailsv;
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c) {
            if (start[r][c] == final[r][c])
                continue;
            bool swaph{}, swapv{};
            const int pos = r * C + c;
            if (tailsh.count(pos) == 0 && tailsv.count(pos) == 0) {
                if (c < C - 1 && tailsv.count(pos + 1) == 0 && start[r][c] != start[r][c + 1] && final[r][c] != final[r][c + 1]) {
                    tailsh.insert(pos + 1);
                    swaph = true;
                    ++swapsh;
                } else if (r < R - 1 && tailsh.count(pos + C) == 0 && start[r][c] != start[r + 1][c] && final[r][c] != final[r + 1][c]) {
                    tailsv.insert(pos + C);
                    swapv = true;
                    ++swapsv;
                }
            }
            ++flips;
        }

    std::vector<int> tails;
    std::set_intersection(tailsh.begin(), tailsh.end(), tailsv.begin(), tailsv.end(), std::back_inserter(tails));

    const int swaps = std::min(swapsh + swapsv - int(tails.size()), flips / 2);
    std::cout << flips - swaps;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve_set1_incorrect(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 retiling.cpp -o retiling.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address retiling.cpp -o retiling

Run:
retiling.exe < retiling.in

Input:

3
2 4 1 1
MGMG
MMMG
GMGM
MMMM
3 3 1 1
MGG
GMG
MMM
MMM
MGM
MMG
1 5 1000 1
MGGGG
GGGMM

Output:


*/

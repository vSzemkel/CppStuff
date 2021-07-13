
#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

// Retiling
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000435915/00000000007dc2de

using bipartite_t = std::vector<std::vector<bool>>;

static bool bpm(const bipartite_t& bg, const int candidate, std::vector<int>& matched, std::vector<bool>& seen) {
    const int match_size = int(bg[0].size());
    for (int m = 0; m < match_size; ++m)
        if (bg[candidate][m] && !seen[m]) {
            seen[m] = true;
            if (matched[m] < 0 || bpm(bg, matched[m], matched, seen)) {
                matched[m] = candidate;
                return true;
            }
        }
    return false;
}

static int max_matching(const bipartite_t& bg) {
    if (bg.empty() || bg[0].empty())
        return 0;
    std::vector<int> matched(bg[0].size(), -1);
    int ret{0};
    for (int i = 0; i < int(bg.size()); ++i) {
        std::vector<bool> seen(matched.size());
        if (bpm(bg, i, matched, seen))
            ++ret;
    }
    return ret;
}

static void solve_set1() { // need maximum matching in bipartite graph for this
    int R, C, F, S; std::cin >> R >> C >> F >> S;
    std::vector<std::string> start(R), final(R);
    for (auto& r : start)
        std::cin >> r;
    for (auto& r : final)
        std::cin >> r;

    // construct bipartite graph
    int M{0}, G{0}, flips{0};
    std::vector<std::vector<int>> order(R, std::vector<int>(C));
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c)
            if (start[r][c] == 'M') 
                order[r][c] = M++;
            else
                order[r][c] = G++;
    bipartite_t bg(M, std::vector<bool>(G)); // M -> G
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c) {
            if (start[r][c] == final[r][c])
                continue;
            if (c < C - 1 && start[r][c] != start[r][c + 1] && final[r][c] != final[r][c + 1]) {
                if (start[r][c] == 'M')
                    bg[order[r][c]][order[r][c + 1]] = true;
                else
                    bg[order[r][c + 1]][order[r][c]] = true;
            }
            if (r < R - 1 && start[r][c] != start[r + 1][c] && final[r][c] != final[r + 1][c]) {
                if (start[r][c] == 'M')
                    bg[order[r][c]][order[r + 1][c]] = true;
                else
                    bg[order[r + 1][c]][order[r][c]] = true;
            }
            ++flips;
        }

    int swaps = max_matching(bg);
    swaps = std::min(swaps, flips / 2);
    std::cout << flips - swaps;
}

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
            const int pos = r * C + c;
            if (tailsh.count(pos) == 0 && tailsv.count(pos) == 0) {
                if (c < C - 1 && tailsv.count(pos + 1) == 0 && start[r][c] != start[r][c + 1] && final[r][c] != final[r][c + 1]) {
                    tailsh.insert(pos + 1);
                    ++swapsh;
                } else if (r < R - 1 && tailsh.count(pos + C) == 0 && start[r][c] != start[r + 1][c] && final[r][c] != final[r + 1][c]) {
                    tailsv.insert(pos + C);
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
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
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

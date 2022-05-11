
#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// Hamiltonian Tour
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008caa74/0000000000acf318

const char* map = "SENW";
std::unordered_map<char, int> dir = {{'S', 0}, {'E', 1}, {'N', 2}, {'W', 3}};

static std::string chunk(const char from, const char to) {
    switch ((dir[to] - dir[from] + 4) % 4) {
        case 0:
            return std::string(2, from);
        case 1:
            return std::string(1, from).append(2, to);
        case 2:
            return std::string(1, from).append(1, map[(dir[from] + 1) % 4]).append(2, to);
        case 3:
            return std::string(1, to);
    }

    return {};
}

static void solve() {
    int R, C;
    std::cin >> R >> C;
    const int SZ = R * C;
    std::vector<bool> seen(SZ);
    for (int i = 0; i < SZ; ++i) {
        char c; std::cin >> c;
        seen[i] = c == '#';
    }
    const int dr[4] = {1, 0, -1, 0}; // row "SENW"
    const int dc[4] = {0, 1, 0, -1}; // col
    const int right_wall[4] = {-1, 0, 1, 2};
    const int total_free = SZ - std::count_if(seen.begin(), seen.end(), [](bool b){ return b; });
    std::vector<std::pair<int, int>> stack(1, {0, 3}); // {cell, transition}, assume that first one came with E transition
    std::vector<int> order;
    seen[0] = true;
    while (!stack.empty()) {
        int tour, next{-1};
        const auto& [cell, last] = stack.back();
        const div_t rc = div(cell, C);
        const int& r = rc.quot, c = rc.rem;
        for (const int turn : right_wall) {
            tour = (last + turn + 4) % 4;
            const int s = r + dr[tour], d = c + dc[tour];
            if (~s && s < R && ~d && d < C && !seen[s * C + d]) {
                next = s * C + d;
                seen[next] = true;
                break;
            }
        }

        order.push_back(cell);
        if (~next)
            stack.push_back({next, tour});
        else
            stack.pop_back();
    }

    if (!std::all_of(seen.begin(), seen.end(), [](const bool b){ return b; })) {
        std::cout << "IMPOSSIBLE";
        return;
    }

    const int size = int(order.size());
    if (size == 1) {
        std::cout << map;
        return;
    }

    std::string path;
    div_t rc = div(order[0], C);
    int& pr = rc.quot, pc = rc.rem;
    for (int i = 1; i < size; ++i) {
        const div_t rc = div(order[i], C);
        const int& r = rc.quot, c = rc.rem;
        const int dr = r - pr, dc = c - pc;
        if (dr == 1 && dc == 0)
            path += 'S';
        if (dr == 0 && dc == 1)
            path += 'E';
        if (dr == -1 && dc == 0)
            path += 'N';
        if (dr == 0 && dc == -1)
            path += 'W';
        pr = r; pc = c;
    }

    std::string ans;
    ans.reserve(SZ << 2);
    ans = (path[0] == 'S') ? "SS" : "SEE";
    for (int i = 1; i < int(path.size()); ++i)
        ans += chunk(path[i - 1], path[i]);
    ans.append((path.back() == 'N') ? "NW" : "W");

    assert((total_free << 2) == int(ans.size()));
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
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 hamiltonian_tour.cpp -o hamiltonian_tour.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address hamiltonian_tour.cpp -o hamiltonian_tour

Run:
hamiltonian_tour.exe < hamiltonian_tour.in

Input:

7
1 1
*
2 2
**
*#
3 4
****
*#*#
****
3 1
*
*
#
1 3
*#*
3 4
**#*
**#*
****
3 5
*****
*#*#*
*****

Output:

Case #1: SENW
Case #2: SSSENNEENWWW
Case #3: SSSSSEEEEEEENWWNNNEENWWWWWSEESSSWWWNNNNW
Case #4: SSSENNNW
Case #5: IMPOSSIBLE
Case #6: SSSSSEEEEEEENNNNNWSSSSWWWNNNNWSSSSWNNNNW
Case #7: SSSSSEEEEEEEEENNNNNWWWWWWWSEESSENNEEESSSWWWWWWWNNNNW

*/

/*
ID: marcin.8
LANG: C++
TASK: holstein
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=Nk3Y10gNC1W&S=holstein
*/

#include <algorithm>
#include <fstream>
#include <vector>

std::ifstream task_in("holstein.in");
std::ofstream task_out("holstein.out");

// Nonrecursive version in ./holstein.cpp
// Recursive version in holstein_rq.cpp is faster

int V, G, ans_len;
std::vector<int> ans, cur, to_find, require;
std::vector<std::vector<int>> feeds;

/**
 * @cur_len - size of currently considered set
 * @next - next feed to try
 **/
static void solve(const int cur_len, const int next) {
    if (std::all_of(to_find.begin(), to_find.end(), [](const int r){ return r <= 0; })) {
        ans = cur;
        ans_len = cur_len;
        return;
    }

    const int next_len = cur_len + 1;
    for (int n = next; n < G && next_len < ans_len; ++n) {
        cur[cur_len] = n;
        std::transform(to_find.begin(), to_find.end(), feeds[n].begin(), to_find.begin(), [](const int r, const int f){ return r - f; });
        solve(next_len, n + 1);
        std::transform(to_find.begin(), to_find.end(), feeds[n].begin(), to_find.begin(), [](const int r, const int f){ return r + f; });
    }
}

int main(int, char**)
{
    task_in >> V;
    require.resize(V);
    for (auto& r :require)
        task_in >> r;
    task_in >> G;
    feeds.assign(G, std::vector<int>(V));
    for (auto& f : feeds)
        for (auto& v : f)
            task_in >> v;

    ans.resize(G + 1, G);
    cur.resize(G);
    ans_len = G + 1;
    to_find = require;
    solve(0, 0);

    task_out << ans_len;
    for (int i = 0; i < ans_len; ++i)
        task_out << ' ' << ans[i] + 1;
    task_out << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 holstein_rq.cpp -o holstein.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address holstein_rq.cpp -o holstein

Input:

4
100 200 300 400
3
50   50  50  50
200 300 200 300
900 150 389 399

Output:

2 1 3

*/

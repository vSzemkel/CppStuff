/*
ID: marcin.8
LANG: C++
TASK: holstein
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=Nk3Y10gNC1W&S=holstein
*/

#include <algorithm>
#include <fstream>
#include <numeric>
#include <utility>
#include <vector>

std::ifstream task_in("holstein.in");
std::ofstream task_out("holstein.out");


// Recursive version in ./holstein_rq.cpp
// Recursive version in holstein_rq.cpp is faster

int main(int, char**)
{
    int V, G;
    task_in >> V;
    std::vector<int> require(V);
    for (auto& r :require)
        task_in >> r;
    std::vector<std::vector<int>> feeds;
    task_in >> G;
    feeds.assign(G, std::vector<int>(V));
    for (auto& f : feeds)
        for (auto& v : f)
            task_in >> v;

    std::vector<int> perm(G), to_find(V);
    for (int c = 1; c <= G; ++c) { // search for c-feed combination
        perm.assign(G, 1);
        for (int i = 0; i < c; ++i)
            perm[i] = 0; // invert 0 and 1, 0 means taken
        do {
            auto to_find = require;
            for (int found = 0, pos = 0; found < c; ++pos)
                if (perm[pos] == 0) {
                    std::transform(to_find.begin(), to_find.end(), feeds[pos].begin(), to_find.begin(), [](const int r, const int f){ return r - f; });
                    ++found;
                }
            if (std::all_of(to_find.begin(), to_find.end(), [](const int r){ return r <= 0; })) {
                task_out << c;
                for (int f = 0; f < G; ++f)
                    if (perm[f] == 0)
                        task_out << ' ' << f + 1;
                task_out << '\n';
                exit(0);
            }
        } while (std::next_permutation(perm.begin(), perm.end()));
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 holstein.cpp -o holstein.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address holstein.cpp -o holstein

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

/*
ID: marcin.8
LANG: C++
TASK: bigbrn
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=FWfFlfCOnfs&S=bigbrn
*/

#include <fstream>
#include <vector>

std::ifstream task_in("bigbrn.in");
std::ofstream task_out("bigbrn.out");

int main(int, char**)
{
    int N, T;
    std::string comet, group;
    task_in >> N >> T;
    std::vector<std::vector<bool>> trees(N, std::vector<bool>(N));
    for (; T; --T) {
        int r, c;
        task_in >> r >> c;
        trees[--r][--c] = true;
    }

    int ans;
    std::vector<std::vector<bool>> barns(N, std::vector<bool>(N, true));
    for (ans = 0; ans < N; ++ans) {
        bool found{};
        for (int r = 0; r < N - ans; ++r)
            for (int c = 0; c < N - ans; ++c)
                if (barns[r][c] && barns[r + ans][c + ans] && !trees[r + ans][c] && !trees[r][c + ans])
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

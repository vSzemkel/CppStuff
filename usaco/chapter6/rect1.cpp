/*
ID: marcin.8
LANG: C++
TASK: rect1
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=j4kadQZ1Cxc&S=rect1
*/

#include <algorithm>
#include <array>
#include <fstream>
#include <map>
#include <vector>

std::ifstream task_in("rect1.in");
std::ofstream task_out("rect1.out");

void solution1()
{
    const int WHITE = 1;
    int R, C, N, total{};
    task_in >> C >> R >> N;
    std::map<int, int> ans;
    std::vector<int> taken(R);
    std::vector<std::vector<bool>> board(R);

    std::vector<std::array<int, 5>> rect(N); // llx, lly, urx, ury, color
    for (auto& r : rect)
        task_in >> r[0] >> r[1] >> r[2] >> r[3] >> r[4];

    for (int i = N - 1; ~i; --i) {
        auto& s = rect[i];
        if (s[2] < s[0])
            std::swap(s[2], s[0]);
        if (s[3] < s[1])
            std::swap(s[3], s[1]);

        for (int r = s[1]; r < s[3]; ++r) {
            auto& row = board[r];
            if (row.empty())
                row.resize(C);
            std::fill_n(row.begin() + s[0], s[2] - s[0], true);
            const auto ntaken = std::count_if(row.begin(), row.end(), [](const bool value){ return value; });
            const auto d = ntaken - taken[r];
            if (s[4] != WHITE && 0 < d) {
                total += d;
                ans[s[4]] += d;
            }
            taken[r] = ntaken;
        }
    }

    task_out << "1 " << R * C - total << '\n';
    for (const auto& [color, count] : ans)
        task_out << color << ' ' << count << '\n';
}

int main(int, char**)
{
    solution1();
}
/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 rect1.cpp -o rect1.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address rect1.cpp -o rect1

Run:
rect1.exe && type rect1.out

Input:

20 20 3
2 2 18 18 2
0 8 19 19 3
8 0 10 19 4

Output:

1 91
2 84
3 187
4 38

*/

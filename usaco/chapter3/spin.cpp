/*
ID: marcin.8
LANG: C++
TASK: spin
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=reZJpd9ODRo&S=spin
*/

#include <algorithm>
#include <array>
#include <fstream>
#include <vector>

std::ifstream task_in("spin.in");
std::ofstream task_out("spin.out");

constexpr const int N = 5;

/**
 * @brief The key observation for this problem is that after 360 seconds, the wheels have returned to their original locations
 */
int main(int, char**)
{
    std::array<int, N> speed;
    std::array<int, N> extents;
    std::array<std::vector<std::array<int, 2>>, N> wedges;
    for (int w = 0; w < N; ++w) {
        task_in >> speed[w];
        task_in >> extents[w];
        wedges[w].resize(extents[w]);
        for (int j = 0; j < extents[w]; ++j)
            task_in >> wedges[w][j][0] >> wedges[w][j][1];
        std::sort(wedges[w].begin(), wedges[w].end());
    }

    for (int t = 0; t < 360; ++t) {
        std::vector<bool> visible(360);
        // visible throw first wheel
        const int pos1 = (speed[0] * t) % 360;
        for (const auto q : wedges[0]) {
            for (int k = 0; k <= q[1]; ++k)
                visible[(q[0] + pos1 + k) % 360] = true;
        }
        // blended by subsequent wheel
        for (int w = 1; w < N; ++w) {
            const int pos = (speed[w] * t) % 360;
            for (int e = 0; e < extents[w]; ++e) {
                const int start = (wedges[w][e][0] + wedges[w][e][1] + 1) % 360;
                const int end = (wedges[w][(e + 1) % extents[w]][0] - 1 + 360) % 360;
                int len = end - start + 1;
                if (len <= 0)
                    len = (361 - start + end) % 360;
                for (int k = 0; k < len; ++k)
                    visible[(start + pos + k) % 360] = false;
            }
        }
        // check
        if (std::any_of(visible.begin(), visible.end(), [](const bool b){ return b; })) {
            task_out << t << '\n';
            return 0;
        }
    }

    task_out << "none\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 spin.cpp -o spin.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address spin.cpp -o spin

Input:

30 1 0 120
50 1 150 90
60 1 60 90
70 1 180 180
90 1 180 60

Output:

9

*/

/*
ID: marcin.8
LANG: C++
TASK: rockers
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=M8dKihAaeaL&S=rockers
*/

#include <fstream>
#include <vector>

std::ifstream task_in("rockers.in");
std::ofstream task_out("rockers.out");

int N, T, M; // num of songs, disk capacity, max number of disks
std::vector<int> items;

void full_search() { // O(2^N * N)
    int ans{0};
    const int SETS = 1 << N;
    for (int sel = 1; sel < SETS; ++sel) {
        int pos{0}, ret{0}, mask{1};
        for (int z = M; z; --z) {
            int sum{0};
            while (pos < N) {
                if (sel & mask) {
                    if (sum + items[pos] > T)
                        break;
                    sum += items[pos];
                    ++ret;
                }
                ++pos;
                mask <<= 1;
            }
        }
        if (ans < ret)
            ans = ret;
    }

    task_out << ans << '\n';
}

int main(int, char**)
{
    task_in >> N >> T >> M;
    items.resize(N);
    for (auto& s : items)
        task_in >> s;

    full_search();
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 rockers.cpp -o rockers.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address rockers.cpp -o rockers

Input:

20 20 10 
18 15 16 10 2 20 14 17 3 7 16 15 18 16 20 16 13 9 4 16 
0b11110000001111111111
18 15 16 10 2 20 14 17 3 7 *16 *15 *18 *16 *20 *16 13 9 4 16 

Output:

14

*/

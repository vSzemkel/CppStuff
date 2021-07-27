/*
ID: marcin.8
LANG: C++
TASK: combo
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=OvbKpDaA6bW&S=combo
*/

#include <fstream>
#include <set>

std::ifstream task_in("combo.in");
std::ofstream task_out("combo.out");

constexpr int BASE = 100;
constexpr int BASE2 = BASE * BASE;
constexpr int TOLLERANCE = 2;
constexpr int WINDOW = 2 * TOLLERANCE + 1;

// Without loose of generality we can replace each number X on the lock with X-1

int main(int, char**)
{
    int N; task_in >> N;
    std::set<int> result;
    for (int g = 0; g < 2; ++g) {
        int a, b, c; task_in >> a >> b >> c;
        for (int i = 0; i < WINDOW; ++i)
            for (int j = 0; j < WINDOW; ++j)
                for (int k = 0; k < WINDOW; ++k)
                    result.insert((a + i) % N * BASE2 + (b + j) % N * BASE + (c + k) % N);
    }

    task_out << result.size() << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 combo.cpp -o combo.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 combo.cpp -o combo

Input:

50
1 2 3
49 6 7

Output:

247

*/

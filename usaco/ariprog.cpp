/*
ID: marcin.8
LANG: C++
TASK: ariprog
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?S=ariprog&a=JG8ONJjSzJv
*/

#include <algorithm>
#include <fstream>
#include <utility>
#include <vector>

std::ifstream task_in("ariprog.in");
std::ofstream task_out("ariprog.out");

using seq_t = std::pair<int, int>; // {b, a}

std::vector<int> target;
std::vector<bool> checker;

int main(int, char**)
{
    int N, M; task_in >> N >> M;
    // generate p*p + q*q set
    checker.resize(2 * M * M);
    for (int p = 0; p <= M; ++p)
        for (int q = p; q <= M; ++q) {
            const int v = p * p + q * q;
            if (!checker[v]) {
                checker[v] = true;
                target.push_back(v);
            }
        }
    std::sort(target.begin(), target.end());

    const int max = int(target.back());
    const int size = int(target.size());
    const int start_ub = size - N + 1;
    std::vector<seq_t> ret; // {b, a}
    for (int i = 0; i < start_ub; ++i) {
        const int a{target[i]};
        for (int j = i + 1; j < size; ++j) {
            const int b{target[j] - a};
            int c = a + (N - 1) * b;
            if (c > max)
                break;
            int len{2};
            while (len < N) {
                if (!checker[c])
                    break;
                c -= b;
                ++len;
            }
            if (len == N)
                ret.emplace_back(b, a);
        }
    }

    if (ret.empty())
        task_out << "NONE\n";
    else {
        std::sort(ret.begin(), ret.end());
        for (const auto& [b, a] : ret)
            task_out << a << ' ' << b << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 ariprog.cpp -o ariprog.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address ariprog.cpp -o ariprog

Input:

21 200

Output:

1217 84
2434 168
4868 336
6085 420
9736 672
10953 756
12170 840
12953 924
15821 1092

*/

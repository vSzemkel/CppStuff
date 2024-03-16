/*
ID: marcin.8
LANG: C++
TASK: milk4
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=V55vnzYsvfC&S=milk4
*/

#include <algorithm>
#include <fstream>
#include <vector>

std::ifstream task_in("milk4.in");
std::ofstream task_out("milk4.out");

int Q;
std::vector<int> pails;

bool check_compination(const std::vector<int>& candidates)
{
    return false;
}

int main(int, char**)
{
    int N;
    task_in >> Q >> N;
    pails.resize(N);
    for (auto&p : pails)
        task_in >> p;
    std::sort(pails.begin(), pails.end(), std::greater<>{});

    std::vector<int> selection(N), candidates;
    for (int count = 1; count <= N; ++count) {
        std::fill_n(selection.begin(), N - count, 0);
        std::fill_n(selection.begin() + N - count, count, 1);
        do {
            candidates.clear();
            for (int i = N - 1; ~i; --i)
                if (selection[i])
                    candidates.push_back(pails[i]);
            if (check_compination(candidates)) {
                task_out << count << ' ';
                for (const auto& p : candidates)
                    task_out << p << ' ';
            }
        } while (std::next_permutation(selection.begin(), selection.end()));
    }
    task_out << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 milk4.cpp -o milk4.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address milk4.cpp -o milk4

Input:

8 4
E2
A6
G1
F5

Output:

33

*/

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

/**
 * Checks if target can be represented as first count candidates linear combiation with + only
*/
bool check_combination(const std::vector<int>& candidates, int count, int target)
{
    const auto& value = candidates[--count];
    if (count == 0)
        return target % value == 0;

    for (int ntarget = target - value; ntarget > 0; ntarget -= value)
        if (check_combination(candidates, count, ntarget))
            return true;

    return false;
}

int main(int, char**)
{
    int Q, N;
    task_in >> Q >> N;
    std::vector<int> pails(N);
    for (auto& p : pails)
        task_in >> p;
    std::sort(pails.begin(), pails.end());

    std::vector<int> selection(N, 1), candidates(N);
    for (int count = 1; count <= N; ++count) {
        selection[count - 1] = 0; // std::fill_n(selection.begin(), count, 0);std::fill_n(selection.begin() + count, N - count, 1);
        do {
            auto cur = candidates.begin();
            for (int i = 0; i < N; ++i)
                if (!selection[i])
                    *cur++ = pails[i];
            if (check_combination(candidates, count, Q)) {
                task_out << count;
                candidates.resize(count);
                for (const auto& p : candidates)
                    task_out << ' ' << p;
                task_out << '\n';
                return 0;
            }
        } while (std::next_permutation(selection.begin(), selection.end()));
    }
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

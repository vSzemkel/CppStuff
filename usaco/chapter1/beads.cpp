/*
ID: marcin.8
LANG: C++
TASK: beads
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=5cWTztbLlz2&S=beads
*/

#include <algorithm>
#include <iostream>
#include <fstream>

std::ifstream task_in("beads.in");
std::ofstream task_out("beads.out");

int solve() // complicated, in-place
{
    int size; task_in >> size;
    std::string necklace; task_in >> necklace;

    int ret{0};
    while (ret < size && necklace[ret] == 'w')
        ++ret;
    if (ret == size)
        return size;

    const char c = necklace[ret];
    while (ret < size && (necklace[ret] == 'w' || necklace[ret] == c))
        ++ret;
    if (ret == size)
        return size;
    else
        std::rotate(necklace.begin(), necklace.begin() + ret, necklace.end());

    int pos1{0};
    const char c1 = necklace[pos1];
    while (pos1 < size && (necklace[pos1] == 'w' || necklace[pos1] == c1))
        ++pos1;

    int checked{0};
    while (checked < size) {
        int pos2{pos1};
        const char c2 = necklace[pos2];
        while (pos2 < size && (necklace[pos2] == 'w' || necklace[pos2] == c2))
            ++pos2;

        while (pos1 > 0 && necklace[pos1 - 1] == 'w')
            --pos1;
        checked += pos1;
        std::rotate(necklace.begin(), necklace.begin() + pos1, necklace.end());
        pos1 = pos2 - pos1;

        ret = std::max(ret, pos2);
    }

    return ret;
}

int solve_smart() // simple, O(N) additional memory
{
    int size; task_in >> size;
    std::string necklace; task_in >> necklace;

    int ret{0}, pos{0};
    necklace += necklace;
    while (pos < size && necklace[pos] == 'w') ++pos;
    if (pos == size) return size;
    const char c = necklace[pos];
    while (pos < size && (necklace[pos] == 'w' || necklace[pos] == c)) ++pos;
    if (pos == size) return size;

    int first_chunk{pos}, i{pos};
    while (i < size) {
        const char c2 = necklace[i];
        while (i < pos + size && (necklace[i] == 'w' || necklace[i] == c2)) ++i;
        if (i == pos + size) return size;
        ret = std::max(ret, first_chunk + i - pos);

        int bonus{0};
        while (pos - bonus - 1 > 0 && necklace[pos - bonus - 1] == 'w') ++bonus;
        first_chunk = i - pos + bonus;
        pos = i;
    }

    return ret;
}

int main(int, char**)
{
    task_out << solve_smart() << "\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 beads.cpp -o beads.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 beads.cpp -o beads.o

Input:

29
wwwbbrwrbrbrrbrbrwrwwrbwrwrrb

Output:

11

*/

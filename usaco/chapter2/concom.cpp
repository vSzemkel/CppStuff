/*
ID: marcin.8
LANG: C++
TASK: concom
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=pSWEHiq3Cii&S=concom
*/

#include <algorithm>
#include <array>
#include <assert.h>
#include <bitset>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <filesystem>
#include <fstream>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <stdlib.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <vector>

std::ifstream task_in("concom.in");
std::ofstream task_out("concom.out");

constexpr int MAX = 100;

int main(int, char**)
{
    std::set<int> owners, all;
    std::array<std::array<int, MAX + 1>, MAX + 1> shares{};
    int N; task_in >> N;
    for (int i = 0; i < N; ++i) {
        int owner, comp, share;
        task_in >> owner >> comp >> share;
        owners.insert(owner);
        all.insert(comp);
        shares[owner][owner] = 100;
        shares[comp][comp] = 100;
        shares[owner][comp] = share;
    }
    all.insert(owners.begin(), owners.end());

    // find owning path from start to finish of length z
    for (int z = 2; z <= int(owners.size()); ++z)
        for (const int start : owners)
            for (const int finish : all)
                if (shares[start][finish] <= 50) {
                    int sum{0};
                    for (const int mid : owners)
                        if (shares[start][mid] > 50)
                            sum += shares[mid][finish];
                    if (sum > 50)
                        shares[start][finish] = 100;
                }

    for (const int i : owners)
        for (const int j : all)
            if (i != j && shares[i][j] > 50)
                task_out << i << ' ' << j << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 concom.cpp -o concom.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address concom.cpp -o concom

Input:

7 
2 3 25 
1 4 36 
4 5 63 
2 1 48 
3 4 30 
4 2 52 
5 3 30

Output:

4 2
4 3
4 5

*/

/*
ID: marcin.8
LANG: C++
TASK: ariprog
PROBLEM STATEMENT: 
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

std::ifstream task_in("ariprog.in");
std::ofstream task_out("ariprog.out");

int main(int, char**)
{
    int N, M; task_in >> N >> M;
    // generate p*p + q*q set
    std::set<int> target;
    for (int p = 0; p <= M; ++p)
        for (int q = 0; q <= M; ++q)
            target.insert(p * p + q * q);

    const int MAX = 2 * M * M;
    std::vector<std::pair<int, int>> ret; // {b, a}
    for (const int a : target) {
        // a + (n-1)*b <= MAX
        const int ub = (MAX - a) / (N - 1);
        for (int b = 1; b <= ub; ++b) {
            int s{1}, c{a + b};
            while (s < N && target.count(c) > 0) {
                c += b;
                ++s;
            }
            if (s == N)
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


Output:


*/

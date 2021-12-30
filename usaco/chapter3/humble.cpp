/*
ID: marcin.8
LANG: C++
TASK: humble
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=C7pFl5TCSr7&S=humble
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

std::ifstream task_in("humble.in");
std::ofstream task_out("humble.out");

int main(int, char**)
{
    int N, Z; task_in >> N >> Z;
    std::vector<int> primes(N);
    std::priority_queue<int64_t, std::vector<int64_t>, std::greater<int64_t>> pq;
    for (auto& p : primes) {
        task_in >> p;
        pq.push(p);
    }

    int64_t ans, prev{-1};
    for (int z = Z; z; --z) {
        ans = pq.top(); pq.pop();
        if (prev < ans) {
            prev = ans;
            for (const auto p : primes)
                pq.push(ans * p);
        } else
            ++z;
    }

    task_out << ans << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 humble.cpp -o humble.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address humble.cpp -o humble

Input:

4 19
2 3 5 7

Output:

27

*/

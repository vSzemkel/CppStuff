/*
ID: marcin.8
LANG: C++
TASK: nuggets
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=1wgmYQz2BNU&S=nuggets
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

std::ifstream task_in("nuggets.in");
std::ofstream task_out("nuggets.out");

int N;
std::vector<int> boxes;

static int64_t gcd(int64_t a, int64_t b)
{
    while (true) {
        if (a == 0) return b;
        b %= a;
        if (b == 0) return a;
        a %= b;
    }
}

static int64_t lcm(int64_t a, int64_t b)
{
    int64_t temp = gcd(a, b);

    return temp ? (a / temp * b) : 0;
}

static int64_t decompose(int64_t c) {
    std::vector<bool> state(c);
    state[0] = true;
    for (int i = 0; i < c; i++) {
        if (!state[i])
            continue;
        for (int j = 0; j < N; ++j) {
            const int sum = i + boxes[j];
            if (sum < c)
                state[sum] = true;
        }
    }

    while (state[--c])
        ;

    return c;
}

int main(int, char**)
{
    task_in >> N;
    boxes.resize(N);
    for (auto& b : boxes)
        task_in >> b;

    int g = boxes.front();
    int64_t m = g;
    for (int i = 1; i < N; ++i) {
        g = gcd(g, boxes[i]);
        m = lcm(m, boxes[i]);
    }

    if (g > 1 || m == 1) {
        task_out << "0\n";
        return 0;
    }

    task_out << decompose(m) << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 nuggets.cpp -o nuggets.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address nuggets.cpp -o nuggets

Input:


Output:


*/

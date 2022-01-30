/*
ID: marcin.8
LANG: C++
TASK: range
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=qyzwTp4qKKA&S=range
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

std::ifstream task_in("range.in");
std::ofstream task_out("range.out");

int main(int, char**)
{
    int N; task_in >> N;
    std::vector<std::string> field(N);
    for (auto& f : field)
        task_in >> f;

    for (int w = 2; w <= N; ++w) {
        int cnt{0};
        for (int r = 0; r <= N - w; ++r)
            for (int c = 0; c <= N - w; ++c)
                if (field[r][c] == '1' && field[r + 1][c] == '1' && field[r][c + 1] == '1'  && field[r + 1][c + 1] == '1')
                    ++cnt;
                else
                    field[r][c] = '0';
        if (cnt)
            task_out << w << ' ' << cnt << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 range.cpp -o range.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address range.cpp -o range

Input:


Output:


*/

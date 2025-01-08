/*
ID: marcin.8
LANG: C++
TASK: clocks
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=XgCDvqBWYNl&S=clocks
*/

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdlib>
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
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <vector>

std::ifstream task_in("clocks.in");
std::ofstream task_out("clocks.out");

static const std::string moves[9] = {"ABDE", "ABC", "BCEF", "ADG", "BDEFH", "CFI", "DEGH", "GHI", "EFHI"};

using state_t = std::array<int, 9>;
static const state_t FINAL{};

struct node_t {
    state_t state;
    std::string path;
};

int main(int, char**)
{
    state_t init;
    for (int row = 0; row < 3; ++row)
        for (int col = 0; col < 3; ++col) {
            int clock; task_in >> clock;
            init[row * 3 + col] = clock / 3 % 4;
        }

    std::queue<node_t> q;
    q.push(node_t{init, {}});
    while (true) {
        const auto& cur = q.front();
        if (cur.state == FINAL) {
            if (!cur.path.empty()) {
                const int l = int(cur.path.size() - 1);
                for (int p = 0; p < l; ++p)
                    task_out << cur.path[p] << ' ';
                task_out << cur.path.back();
            }
            task_out << '\n';
            return 0;
        }
        q.pop();
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 clocks.cpp -o clocks.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address clocks.cpp -o clocks

Run:
clocks.exe && type clocks.out

Input:


Output:


*/

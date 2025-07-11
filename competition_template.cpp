
#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <filesystem>
#include <format>
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

// $TASKNAME$
// 

// when the problem has a solution
// what complexity is demanded given the size of data in the last tests set
// write std::cerr diagnostics

static void solve() {
    int64_t ret{0};
    std::cout << ret;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        // /*attach_trap();*/ solve();
        std::cout << std::format("Case #{}: {}\n", g, solve());
        //std::cout << std::format("Case #{}: ", g) << (solve() ? "" : "IM") << "POSSIBLE\n";
        //std::cout << std::format("Case #{}: ", g) << std::fixed << std::setprecision(9); solve(); std::cout << '\n';
    }
}

/*

Format:
clang-format -i $TASKNAME$.cpp

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 $TASKNAME$.cpp
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address $TASKNAME$.cpp

Run:
py.exe interactive_runner.py py.exe $TASKNAME$_testing_tool.py 1 -- a.exe
a.exe < $TASKNAME$.in

Input:


Output:


*/

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

// Suffix Array
// https://cp-algorithms.com/string/suffix-array.html

auto naive_suffix_array(const std::string& s) { // O(N2logN)
    const auto size = s.size();
    std::vector<std::pair<std::string_view, int>> suffixes(size);
    for (size_t i = 0; i < size; ++i)
        suffixes[i] = {std::string_view{s.data() + i, size - i}, i};
    std::sort(suffixes.begin(), suffixes.end());

    std::vector<int> ret(size);
    std::transform(suffixes.cbegin(), suffixes.cend(), ret.begin(), [](const auto& p){ return p.second; });
    return ret;
}

int main(int, char**)
{
    const std::string s = "abaab";
    const auto order = naive_suffix_array(s);
    for (const int o : order)
        std::cout << s.substr(o) << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 suffix_array.cpp -o suffix_array.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address suffix_array.cpp -o suffix_array

Run:
py.exe interactive_runner.py py.exe suffix_array_testing_tool.py 1 -- suffix_array.exe
suffix_array.exe < suffix_array.in

Input:


Output:


*/

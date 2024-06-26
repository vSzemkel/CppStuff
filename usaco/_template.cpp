/*
ID: marcin.8
LANG: C++
TASK: template
PROBLEM STATEMENT: 
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

std::ifstream task_in("template.in");
std::ofstream task_out("template.out");

int main(int, char**)
{
    std::string comet, group;
    task_in >> comet >> group >> std::ws;
    task_out << (decrypt(comet) == decrypt(group) ? "GO" : "STAY") << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 template.cpp -o template.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address template.cpp -o template

Run:
template.exe && type template.out

Input:


Output:


*/

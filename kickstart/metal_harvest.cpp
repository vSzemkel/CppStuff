
#include <iostream>
#include <map>
#include <vector>

template <typename K, typename V> std::map<K, V> fillmap(int size){ std::map<K, V> cont; for (int i = 0; i < size; ++i) { int k, v; std::cin >> k >> v; cont[k] = v;} return cont; };

int g_robot_limit, g_size;
std::map<int, int> g_sections;

int naive() {
    std::vector<bool> harvest;
    for (const auto& s : g_sections) {
        if ((int)harvest.size() < s.second)
            harvest.resize(s.second);
        for (auto j = s.first; j < s.second; ++j) 
            harvest[j] = true;
    }

    int pos{0}, ret{0};
    while (true) {
        while (pos < (int)harvest.size() && !harvest[pos]) ++pos;
        if (pos >= (int)harvest.size()) break;
        ++ret;
        pos += g_robot_limit;
    }

    return ret;
}

int solve() {
    int ret{0}, covered{0};
    for (const auto& s : g_sections) {
        if (s.second <= covered)
            continue;

        const int start = std::max(covered, s.first);
        const int robots = ((s.second - start) + g_robot_limit - 1) / g_robot_limit;
        ret += robots;
        covered = start + (robots * g_robot_limit);
    }

    return ret;
}

int main(int argc, char* argv[])
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    // parse console input
    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cin >> g_size >> g_robot_limit;
        g_sections.clear();
        g_sections = fillmap<int, int>(g_size);
        //std::cout << "Case #" << g << ": " << naive() << "\n";
        std::cout << "Case #" << g << ": " << solve() << "\n";
    }
}

/*
clang++.exe -Wall -ggdb3 -O0 -std=c++17 metal_harvest.cpp -o metal_harvest.exe
metal_harvest.exe < metal_harvest.in

Input:

3
2 5
1 2
5 6
3 5
1 5
10 11
8 9
3 2
1 2
3 5
13 14

Output:

Case #1: 1
Case #2: 2
Case #3: 3

*/

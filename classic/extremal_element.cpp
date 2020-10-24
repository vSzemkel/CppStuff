
#include <algorithm>
#include <iostream>
#include <vector>

// Local extremum in a sequence with no consecutive equal elements in logarithmic time

std::vector<int> g_input;

ptrdiff_t next_max(const int offset) {
    return std::lower_bound(g_input.begin() + offset + 1, g_input.end(), 0, 
        [](const auto& n, auto) { 
            const auto pred = &n - 1;
            return *pred <= n;
        }) - g_input.begin() - 1;
}

ptrdiff_t next_min(const int offset) {
    return std::lower_bound(g_input.begin() + offset + 1, g_input.end(), 0, 
        [](const auto& n, auto) { 
            const auto pred = &n - 1;
            return *pred >= n;
        }) - g_input.begin() - 1;
}

void solve() {
    int nm, offset = next_max(0);
    do {
        std::cout << "[" << offset << "]=" << g_input[offset] << " ";
        nm = next_min(offset + 1);
        if (nm + 1 == g_input.size()) break;
        offset = next_max(++nm);
        if (nm == offset) break;
    } while (true);
}

int main(int argc, char* argv[])
{
    int no_of_cases, size;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cin >> size;
        g_input.resize(size);
        for (auto& c : g_input) std::cin >> c;

        std::cout << "Case #" << g << ": ";
        solve();
        std::cout << "\n";
    }
}

/*
clang++.exe -Wall -ggdb3 -O0 -std=c++17 extremal_element.cpp -o extremal_element.exe
extremal_element.exe < extremal_element.in

Input:

5
10
0 1 2 3 4 5 6 7 3 2
10
0 1 2 3 4 5 6 7 8 9 
10
9 8 7 6 5 4 3 2 1 0
10
0 1 2 3 2 1 2 4 3 1 
5
1 2 1 2 1

Output:

Case #1: [7]=7 
Case #2: [9]=9
Case #3: [0]=9
Case #4: [3]=3 [7]=4
Case #5: [3]=2

*/

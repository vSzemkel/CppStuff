
#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// Sort the Fabrics
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb409/0000000000beefbb

static void solve() {
    int N; std::cin >> N;
    std::vector<std::pair<std::string, int>> colors(N);
    std::vector<std::pair<int, int>> durabs(N);
    int i{0}, d;
    for (auto& f : colors) {
        std::cin >> f.first >> d >> f.second;
        durabs[i] = {d, f.second};
        ++i;
    }

    std::sort(colors.begin(), colors.end());
    std::sort(durabs.begin(), durabs.end());

    std::unordered_map<int, int> index; // {uid -> pos in durabs}
    for (int i = 0; i < N; ++i)
        index[durabs[i].second] = i;
    int ans{0};
    for (int i = 0; i < N; ++i)
        if (index[colors[i].second] == i)
            ++ans;

    std::cout << ans;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 sort_the_fabrics.cpp -o sort_the_fabrics.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address sort_the_fabrics.cpp -o sort_the_fabrics

Run:
sort_the_fabrics.exe < sort_the_fabrics.in

Input:

3
2
blue 2 1
yellow 1 2
2
blue 2 1
brown 2 2
1
red 1 1

Output:

Case #1: 0
Case #2: 2
Case #3: 1

*/

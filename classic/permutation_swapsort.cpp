
#include <assert.h>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

// Permutation swapsort
// Given a permutation sort it with minimum number of swaps
// see: /kickstart/2022/level_design.cpp

constexpr const int size = 35;

template <typename C>
static void print(const C& v)
{
    if (v.empty()) return;
    std::cout << v.front();
    const int size = int(v.size());
    for (int i = 1; i < size; ++i)
        std::cout << ' ' << v[i];
    std::cout << '\n';
}

int main(int, char**)
{
    std::vector<int> data(size);
    std::iota(data.begin(), data.end(), 0);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(data.begin(), data.end(), g);

    std::vector<int> rev_ind(size);
    for (int i = 0; i < size; ++i)
        rev_ind[data[i]] = i;

    // identify cycles
    std::vector<bool> visited(size);
    std::vector<std::vector<int>> cycles;
    for (int i = 0; i < size; ++i) {
        if (visited[i] || data[i] == i)
            continue;
        visited[i] = true;
        cycles.push_back(std::vector(1, i));
        auto& cycle = cycles.back();
        auto next = rev_ind[i];
        while (!visited[next]) {
            visited[next] = true;
            cycle.push_back(next);
            next = rev_ind[next];
        }
    }

    const auto require = std::accumulate(cycles.begin(), cycles.end(), 0, [](const auto acc, const auto& v){ return acc + v.size(); }) - cycles.size();
    assert(size / 2 <= require && require < size);
    std::cout << "Sorting this permutation requires " << require << " swaps\n";

    int counter{0};
    std::cout << std::setw(2) << counter++ << ": "; print(data);
    for (const auto& c : cycles) {
        for (int i = 1; i < int(c.size()); ++i) {
            std::cout << std::setw(2) << counter++ << ": ";
            std::swap(data[c[i - 1]], data[c[i]]);
            print(data);
        }
    }

    assert(std::is_sorted(data.begin(), data.end()));
    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 permutation_swapsort.cpp -o permutation_swapsort.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address permutation_swapsort.cpp -o permutation_swapsort

*/

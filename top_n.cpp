
#include <array>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>

// find n-th lowest element


// case definition - words collection
constexpr int g_size = 2000000;
std::array<int, g_size> g_coll;

void init() 
{
    std::random_device rd;
    std::mt19937 g(rd());

    std::iota(g_coll.begin(), g_coll.end(), 1);
    std::shuffle(g_coll.begin(), g_coll.end(), g);
}

int with_heap(int n) 
{
    const auto& order = std::greater<>{};
    std::make_heap(g_coll.begin(), g_coll.end(), order);
    for (int i = 0; i < n - 1; ++i)
        std::pop_heap(g_coll.begin(), g_coll.end() - i, order);

    return g_coll.front();
}

int with_partition(int n) 
{
    --n; // the first one has really index == 0
    size_t low = 0;
    size_t hi = g_size;
    while (low < hi) {
        const auto key = g_coll[hi - 1];
        const auto it = std::partition(g_coll.begin() + low, g_coll.begin() + hi, [key](const int val){ return val < key; });
        const auto pivot = std::distance(g_coll.begin(), it);
        std::swap(g_coll[pivot], g_coll[hi - 1]);
        if (pivot == n) return *it;
        if (pivot < n) low = pivot + 1;
        else hi = pivot;
    }

    return g_coll.front();
}

int main(int argc, char* argv[])
{
    init();
    std::cout << "512th lowest element is: " << with_heap(512) << "\n";

    init();
    std::cout << "1039561st lowest element is: " << with_partition(1039561) << "\n";

    return 0;
}

/* clang++.exe -Wall -g -std=c++17 top_n.cpp -o top_n.exe

Output:

512th lowest element is: 512
1039561st lowest element is: 1039561

*/
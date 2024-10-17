
#include <iostream>
#include <format>
#include <numeric>
#include <vector>

int counter{};
static constexpr int N = 5;
static constexpr int K = 3;

template <typename C>
static void print(const C& v, std::ostream& task_out = std::cout)
{
    ++counter;
    if (v.empty())
        return;
    char sep = ' ';
    auto lst = v.size();
    for (const auto& e : v) {
        if (--lst == 0) sep = '\n';
        task_out << e << sep;
    }
}

// Toggle Permute Distribute

void toggle(std::vector<bool>& data, const int pos)
{
    if (pos == N)
        print(data);
    else {
        toggle(data, pos + 1);
        data[pos] = !data[pos];
        toggle(data, pos + 1);
        data[pos] = !data[pos];
    }
}

void permute(std::vector<int>& data, const int pos)
{
    if (pos == N)
        print(data);
    else {
        permute(data, pos + 1);
        for (int i = pos + 1; i < N; ++i) {
            std::swap(data[pos], data[i]);
            permute(data, pos + 1);
            std::swap(data[pos], data[i]);
        }
    }
}

void distribute_discrete(std::vector<int>& data, const int pos)
{
    if (pos == N)
        print(data);
    else
        for (int bucket = 0; bucket < K; ++bucket) {
            data[pos] = bucket;
            distribute_discrete(data, pos + 1);
        }
}

void distribute_bitmask(std::vector<int>& data, const int pos)
{
    if (pos == N)
        print(data);
    else {
        const int mask = 1 << pos;
        for (auto& bucket : data) {
            bucket |= mask;
            distribute_bitmask(data, pos + 1);
            bucket &= ~mask;
        }
    }
}

int main(int, char**)
{
    std::vector<bool> toggle_data(N);
    toggle(toggle_data, 0);
    std::cout << std::format("Found {} toggle possibilities.\n\n", counter);

    counter = 0;
    std::vector<int> permute_data(N);
    std::iota(permute_data.begin(), permute_data.end(), 0);
    permute(permute_data, 0);
    std::cout << std::format("Found {} permutations.\n\n", counter);

    counter = 0;
    std::vector<int> distribute_discrete_data(N);
    distribute_discrete(distribute_discrete_data, 0);
    std::cout << std::format("Found {} discrete distributions.\n\n", counter);

    counter = 0;
    std::vector<int> distribute_bitmask_data(K);
    distribute_bitmask(distribute_bitmask_data, 0);
    std::cout << std::format("Found {} bitmask distributions.\n\n", counter);
}

/*

Format:
clang-format -i toggle_permute_distribute.cpp

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 toggle_permute_distribute.cpp -o toggle_permute_distribute.exe

*/

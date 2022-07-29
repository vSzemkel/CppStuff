
#include <cassert>
#include <iostream>
#include <execution>
#include <numeric>
#include <thread>
#include <vector>

// Sum in Threads

constexpr const size_t size = 1'000'000;

std::atomic<int64_t> ans;
std::vector<int> input(size);

static int64_t solve() {
    return std::reduce(std::execution::par, input.begin(), input.end(), int64_t{});
}

static int64_t solve_interview() {
    const auto worker = [&](const int* val, const size_t size) {
        int64_t res{0};
        for (int z = size; z; --z)
            res += *val++;
        ans += res;
    };

    const auto cpu_cnt = int(std::thread::hardware_concurrency());
    const int normal_chunk_size = size / cpu_cnt;
    std::vector<std::thread> joinable(cpu_cnt - 1);
    for (int i = 0; i < cpu_cnt - 1; ++i)
        joinable[i] = std::thread(worker, input.data() + i * normal_chunk_size, normal_chunk_size);

    const int last_chunk_size = size - (cpu_cnt - 1) * normal_chunk_size;
    worker(input.data() + size - last_chunk_size, last_chunk_size);

    for (auto& t : joinable)
        t.join();

    return ans;
}

int main(int, char**)
{
    std::iota(input.begin(), input.end(), 1);
    const auto smart_result = size * (size + 1) / 2;
    const auto result = solve();
    assert(smart_result == result);
    const auto interview_result = solve_interview();
    assert(smart_result == interview_result);
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 sum_in_threads.cpp -o sum_in_threads.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address sum_in_threads.cpp -o sum_in_threads

*/


#include <cassert>
#include <iostream>
#include <execution>
#include <future>
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
    std::vector<std::thread> joinable;
    for (int i = 0; i < cpu_cnt - 1; ++i)
        joinable.emplace_back(worker, input.data() + i * normal_chunk_size, normal_chunk_size);

    const int last_chunk_size = size - (cpu_cnt - 1) * normal_chunk_size;
    worker(input.data() + size - last_chunk_size, last_chunk_size);

    for (auto& t : joinable)
        t.join();

    return ans;
}

static int64_t solve_future() {
    const auto worker = [&](const int* val, const size_t size) {
        int64_t res{0};
        for (int z = size; z; --z)
            res += *val++;
        return res;
    };

    const auto cpu_cnt = int(std::thread::hardware_concurrency());
    const int normal_chunk_size = size / cpu_cnt;
    std::vector<std::future<int64_t>> fut(cpu_cnt - 1);
    for (int i = 0; i < cpu_cnt - 1; ++i)
        fut[i] = std::async(std::launch::async, worker, input.data() + i * normal_chunk_size, normal_chunk_size);

    const int last_chunk_size = size - (cpu_cnt - 1) * normal_chunk_size;
    int64_t ans = worker(input.data() + size - last_chunk_size, last_chunk_size);

    for (auto& f : fut)
        ans += f.get();

    return ans;
}

static int64_t solve_packaged_task() {
    const auto worker = [&](const int* val, const size_t size) {
        int64_t res{0};
        for (int z = size; z; --z)
            res += *val++;
        return res;
    };

    const auto cpu_cnt = int(std::thread::hardware_concurrency());
    const int normal_chunk_size = size / cpu_cnt;
    std::vector<std::future<int64_t>> fut(cpu_cnt - 1);
    for (int i = 0; i < cpu_cnt - 1; ++i) {
        std::packaged_task<int64_t(const int*, size_t)> task{worker};
        fut[i] = task.get_future();
        std::jthread do_work(std::move(task), input.data() + i * normal_chunk_size, normal_chunk_size);
    }

    const int last_chunk_size = size - (cpu_cnt - 1) * normal_chunk_size;
    int64_t ans = worker(input.data() + size - last_chunk_size, last_chunk_size);

    for (auto& f : fut)
        ans += f.get();

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
    const auto future_result = solve_future();
    assert(smart_result == future_result);
    const auto packaged_task_result = solve_packaged_task();
    assert(smart_result == packaged_task_result);
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 sum_in_threads.cpp -o sum_in_threads.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address sum_in_threads.cpp -o sum_in_threads

*/

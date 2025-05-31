
#include <iostream>
#include <format>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

// Solve task cases cuncurrently

static std::string solve(int waste)
{
    return "return";
}

int case_id{};
int no_of_cases;
std::mutex runner_mutex;
std::vector<int> arg;
std::vector<std::string> ans;

// For each CPU run next test case until the backlog is empty
void multithreaded_solve()
{
    while (true) {
        runner_mutex.lock();
        const int cur = case_id;
        if (cur >= no_of_cases) {
            runner_mutex.unlock();
            return;
        }
        ++case_id;
        for (auto& a : arg)
            std::cin >> a;
        runner_mutex.unlock();
        ans[cur] = solve(arg[cur]);
    }
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    std::cin >> no_of_cases;
    arg.resize(no_of_cases);
    ans.resize(no_of_cases);
    const auto cpu_cnt = int(std::thread::hardware_concurrency());

    std::vector<std::thread> joinable;
    for (int i = 0; i < cpu_cnt; ++i)
        joinable.emplace_back(multithreaded_solve);
    for (auto& t : joinable)
        t.join();
    for (int g = 1; g <= no_of_cases; ++g)
        std::cout << std::format("Case #{}: {}\n", g, ans[g - 1]);
}

/*

Format:
clang-format -i multithreaded_solve.cpp.cpp

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 multithreaded_solve.cpp.cpp -o multithreaded_solve.cpp.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address multithreaded_solve.cpp.cpp -o multithreaded_solve.cpp

*/

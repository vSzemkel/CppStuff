
#include <iostream>
#include <chrono>
#include <format>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <windows.h>

// Solve task cases cuncurrently

static std::string solve(const char c, const int length)
{
    static thread_local int counter = 0;

    std::string ret;
    // intentionally slow
    for (int z = length; z; --z)
        ret = ret + c;

    SYSTEMTIME st;
    GetLocalTime(&st);
    const auto thread_id = std::this_thread::get_id();

    return std::format("Thread: {:05}-{}, time: {:04}-{:02}-{:02}T{:02}:{:02}:{:02}.{:03}, result: {}", *(unsigned int*)&thread_id, ++counter, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, ret);
}

int case_id{};
int no_of_cases;
std::mutex runner_mutex;
std::vector<char> arg0;
std::vector<int> arg1;
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

        for (int i = 0; i < no_of_cases; ++i)
            std::cin >> arg0[i] >> arg1[i];

        runner_mutex.unlock();
        ans[cur] = solve(arg0[cur], arg1[cur]);
    }
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    std::cin >> no_of_cases;
    arg0.resize(no_of_cases);
    arg1.resize(no_of_cases);
    ans.resize(no_of_cases);
    const auto cpu_cnt = int(std::thread::hardware_concurrency());

    std::vector<std::thread> joinable;
    for (int i = 0; i < cpu_cnt; ++i)
        joinable.emplace_back(multithreaded_solve);
    for (auto& t : joinable)
        t.join();
    for (int g = 1; g <= no_of_cases; ++g)
        std::cout << std::format("Case #{:02}: {}\n", g, ans[g - 1]);
}

/*

Format:
clang-format -i multithreaded_solve\.cpp

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 multithreaded_solve.cpp -o multithreaded_solve.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address multithreaded_solve.cpp -o multithreaded_solve.cpp

Run:
(multithreaded_solve.exe < multithreaded_solve.in) > multithreaded_solve.out

Input:

20
C 62
P 981
A 612
K 8
O 227
J 16
P 300
B 1025
Z 15
M 712
Q 87
A 829
R 881
N 7
I 711
U 208
E 329
D 90
H 190
F 410
X 619

*/

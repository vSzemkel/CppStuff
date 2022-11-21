
#include <cassert>
#include <chrono>
#include <iostream>
#include <deque>
#include <queue>
#include <random>
#include <thread>
#include <tuple>
#include <vector>


// Cheap Jumps
// Given an array of size N of non negative integrals, you are to find
// a jump sequence from index 0 to N-1 with minimal sum of visited slots
// Single jump has length in range from 1 to J

constexpr const int64_t INF = 1e18;
using jump_t = std::tuple<int64_t, int64_t, int64_t>; // <cost in to, from, to>

int64_t N, J;
std::vector<int64_t> input;

static std::mt19937 g_gen{std::random_device{}()};
static auto cost(const jump_t& j) { return std::get<0>(j); }
static auto dest(const jump_t& j) { return std::get<2>(j); }

template <typename T = int>
static auto distribution(const T ubound) {
    std::uniform_int_distribution<T> dist(0, ubound - 1);
    return dist;
}

struct stopwatch_t {
    stopwatch_t() { _start = std::chrono::high_resolution_clock::now(); }
    ~stopwatch_t() {
        const auto d =  std::chrono::high_resolution_clock::now() - _start;
        if (d.count() > std::giga::num)
            std::cout << std::chrono::duration<double>(d).count() << " sec.";
        else
            std::cout << std::chrono::duration<double, std::milli>(d).count() << " ms.";
    }
  private:
     std::chrono::high_resolution_clock::time_point _start;
};

/**
 * @brief Jump to the current position from rmq result of previous at most J slots
 * 
 * @return Cost of the traversal
 */
static auto solve_rmq(bool supress_msg = false) { // O(N)
    if (N <= J)
        return input.back();

    std::deque<jump_t> dq;
    std::vector<int64_t> prev(N);
    for (int64_t i = 0; i < J; ++i) {
        while (!dq.empty() && input[i] < input[dest(dq.back())])
            dq.pop_back();
        prev[i] = -1;
        dq.emplace_back(input[i], -1, i);
    }
    for (int64_t i = J; i < N; ++i) {
        if (!dq.empty() && J < i - dest(dq.front()))
            dq.pop_front();
        const auto [total, from, to] = dq.front();
        prev[to] = from;
        while (!dq.empty() && total + input[i] < cost(dq.back()))
            dq.pop_back();
        dq.emplace_back(total + input[i], to, i);
    }
    prev[N - 1] = dest(dq.front());

    int64_t slot = N - 1;
    std::vector<int64_t> path;
    while (~slot) {
        path.push_back(slot);
        slot = prev[slot];
    }

    const auto ret = cost(dq.front()) + input.back();
    if (!supress_msg) {
        std::cout << ret << "\nSTART_RMQ -> ";
        for (auto it = path.rbegin(); it != path.rend(); ++it)
            std::cout << (*it) + 1 << " -> ";
        std::cout << "END\n";
    }

    return ret;
}

static auto solve_mst(bool supress_msg = false) { // O(N * J)
    if (N <= J)
        return input.back();

    std::queue<int64_t> qq;
    std::vector<int64_t> prev(N + 1), score(N + 1, INF);
    std::vector<bool> inque(N + 1);
    score[0] = 0;
    prev[0] = -1;
    qq.push(0);
    while (!qq.empty()) {
        const auto cur = qq.front();
        qq.pop();
        inque[cur] = false;
        for (int64_t i = 1; i <= J; ++i) {
            const auto next = cur + i;
            if (next <= N && score[cur] + input[next - 1] < score[next]) {
                score[next] = score[cur] + input[next - 1];
                prev[next] = cur;
                if (!inque[next]) {
                    inque[next] = true;
                    qq.emplace(next);
                }
            }
        }
    }

    int64_t slot = N;
    std::vector<int64_t> path;
    while (~slot) {
        path.push_back(slot);
        slot = prev[slot];
    }
    path.pop_back();

    if (!supress_msg) {
        std::cout << score.back() << "\nSTART_MST -> ";
        for (auto it = path.rbegin(); it != path.rend(); ++it)
            std::cout << *it << " -> ";
        std::cout << "END\n";
    }

    return score.back();
}

static auto solve_brt(bool supress_msg = false) { // O(N * J)
    int64_t ret = input.back();
    if (N <= J)
        return ret;

    std::vector<int64_t> dp(N + 1, INF), prev(N + 1, -1);
    dp[0] = 0;
    for (int64_t i = 1; i <= N; ++i) 
        for (int64_t j = 1; j <= J; ++j)
            if (0 <= i - j && dp[i - j] + input[i - 1] < dp[i]) {
                dp[i] = dp[i - j] + input[i - 1];
                prev[i] = i - j;
            }

    int64_t slot = N;
    std::vector<int64_t> path;
    while (~slot) {
        path.push_back(slot);
        slot = prev[slot];
    }
    path.pop_back();

    if (!supress_msg) {
        std::cout << dp[N] << "\nSTART_BRT -> ";
        for (auto it = path.rbegin(); it != path.rend(); ++it)
            std::cout << *it << " -> ";
        std::cout << "END\n";
    }

    return dp[N];
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cin >> N >> J;
        input.resize(N);
        for (auto& s : input)
            std::cin >> s;

        std::cout << "Case #" << g << ": ";
        const auto ret_mst = solve_mst(true);
        const auto ret_rmq = solve_rmq(true);
        const auto ret_brt = solve_brt();
        assert(ret_mst == ret_rmq && ret_rmq == ret_brt);
    }

    N = 1e06;
    J = 1000;
    input.resize(N);
    auto dist = distribution<int64_t>(1000);
    for (auto& n : input)
        n = dist(g_gen);

    int64_t ret_mst, ret_rmq, ret_brt;
    std::cout << "\nBRT TIME: ";
    {
        stopwatch_t sw;
        ret_brt = solve_brt(true);
    }
    std::cout << "\nMST TIME: ";
    {
        stopwatch_t sw;
        ret_mst = solve_mst(true);
    }
    std::cout << "\nRMQ TIME: ";
    {
        stopwatch_t sw;
        ret_rmq = solve_rmq(true);
    }
    assert(ret_mst == ret_rmq && ret_rmq == ret_brt);
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 cheap_jumps.cpp -o cheap_jumps.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 cheap_jumps.cpp -o cheap_jumps.o

Run:
cheap_jumps.exe < cheap_jumps.in

Input:

3
10 3
1 200 7 162 381 9 76 1 90 7
8 6
1 2 666 666 666 666 666 1
20 4
28 8 16 50 127 0 8 87 1111 876 9 12 7 32 918365896 8 1 90 37 5

Output:

Case #1: 25
0 -> 2 -> 5 -> 7 -> 9 -> END
Case #2: 4
0 -> 1 -> 7 -> END
Case #3: 66
0 -> 1 -> 5 -> 6 -> 10 -> 12 -> 16 -> 19 -> END

BRT TIME: 6.60428 sec.
MST TIME: 6.25823 sec.
RMQ TIME: 277.776 ms.

*/

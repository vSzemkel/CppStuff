
#include <algorithm>
#include <array>
#include <iostream>
#include <format>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

// Wildcard Submissions
// https://www.facebook.com/codingcompetitions/hacker-cup/2024/round-1/problems/E
// kudos to: tourist

// Every input string defines some nodes in trie. We need to count union-set of those
// We will consider every posible subset selection of input strings as a subset and 
// this subset contribution to the union-set as defined by inclusion-exclusion principle.
// For each subset we care only for intersection of trie nodes defined by selected strings,
// meaning common prefix of all strings in the subset.

const int MAX_LEN = 103;
const int64_t M = 998244353;
std::array<int64_t, MAX_LEN> pw; // powers of 26 modulo M

static int solve(const std::vector<std::string>& words)
{
    const int N = int(words.size());

    std::vector<int> lengths(N);
    for (int i = 0; i < N; ++i)
        lengths[i] = int(words[i].size());

    std::vector<std::vector<int>> common_prefix(N, std::vector<int>(N));
    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j) {
            const int common = std::min(lengths[i], lengths[j]);
            for (int k = 0; k < common; ++k) {
                if (words[i][k] != '?' && words[i][k] != '?' && words[i][k] != words[j][k])
                    break;
                ++common_prefix[i][j];
            }
        }

    std::vector<std::vector<bool>> masks(N);
    for (int i = 0; i < N; ++i) {
        const auto& s = words[i];
        const int len = lengths[i];
        auto& mask = masks[i];
        mask.reserve(len);
        for (int j = 0; j < len; ++j)
            mask.push_back(s[j] == '?');
    }

    int64_t ret{1};

    std::function<void(int, int, int, std::vector<bool>, std::vector<bool>)> drill =
    [&](int cur, int taken, int cmn_pref, std::vector<bool> selection, std::vector<bool> wildcards){
        if (cur == N) {
            if (0 < taken) {
                int ord{};
                const int sign = 2 * (taken & 1) - 1;
                for (int i = 0; i < cmn_pref; ++i) {
                    if (wildcards[i])
                        ++ord;
                    ret += sign * pw[ord] % M;
                }
            }
            return;
        }

        // ignore cur
        drill(cur + 1, taken, cmn_pref, selection, wildcards);
        // take cur
        selection[cur] = true;
        const int len = lengths[cur];
        cmn_pref = std::min(cmn_pref, len);
        for (int i = 0; i < cur; ++i)
            if (selection[i])
                cmn_pref = std::min(cmn_pref, common_prefix[i][cur]);
        for (int i = 0; i < len; ++i)
            wildcards[i] = wildcards[i] && masks[cur][i];
        drill(cur + 1, taken + 1, cmn_pref, selection, wildcards);
    };

    drill(0, 0, MAX_LEN, std::vector<bool>(MAX_LEN), std::vector<bool>(MAX_LEN, true));
    return ret;
}

int case_id{};
int no_of_cases;
std::mutex runner_mutex;
std::vector<std::vector<std::string>> arg0;
std::vector<int> ans;

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

        int N; std::cin >> N;
        auto& arg = arg0[cur];
        arg.resize(N);
        for (auto& a : arg)
            std::cin >> a;

        runner_mutex.unlock();
        ans[cur] = solve(arg0[cur]);
    }
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    pw[0] = 1;
    for (int i = 1; i < MAX_LEN; ++i)
        pw[i] = (pw[i - 1] * 26) % M;

    std::cin >> no_of_cases;
    arg0.resize(no_of_cases);
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
clang-format -i wildcard_submissions.cpp

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 wildcard_submissions.cpp -o wildcard_submissions.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address wildcard_submissions.cpp -o wildcard_submissions

Run:
py.exe interactive_runner.py py.exe wildcard_submissions_testing_tool.py 1 -- wildcard_submissions.exe
wildcard_submissions.exe < wildcard_submissions.in

Input:


Output:


*/

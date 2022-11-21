
#include <iostream>
#include <deque>
#include <map>
#include <vector>

// Level Design
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb1b6/0000000000c47792

constexpr const int INF = 1e09;

static void solve() { // koodos to tabr
    int N;
    std::cin >> N;
    std::vector<int> permut(N);
    for (auto& n : permut) {
        std::cin >> n;
        --n;
    }

    // prepare reverse index
    std::vector<int> rev_ind(N);
    for (int i = 0; i < N; ++i)
        rev_ind[permut[i]] = i;

    // identify cycles
    std::vector<bool> visited(N);
    std::map<int, int> cycles_counts; // size -> number of occurences
    for (int i = 0; i < N; ++i) {
        if (visited[i])
            continue;
        visited[i] = true;
        int size{1};
        auto next = rev_ind[i];
        while (!visited[next]) {
            visited[next] = true;
            ++size;
            next = rev_ind[next];
        }
        ++cycles_counts[size];
    }

    std::vector<int> dp(N + 1, INF); // dp[i] cost of making i-cycle
    dp[0] = -1;
    for (const auto& [sz, cnt] : cycles_counts) {
        const int size{sz}; // Clang 15 specific, prevents error: reference to local binding 'size' declared in enclosing function 'solve'
        std::vector<int> ndp(N + 1, INF);
        for (int d = 0; d < size; ++d) { // for every K % size
            std::deque<int> deq;
            for (int it = 0; it * size + d <= N; ++it) { // how many cycles of size x we take
                const auto Get = [&](int when) {
                    return dp[when * size + d] + (it - when);
                };
                while (!deq.empty() && Get(deq.back()) >= Get(it)) {
                    deq.pop_back();
                }
                deq.push_back(it);
                ndp[it * size + d] = Get(deq.front());
                if (deq.front() == it - cnt) {
                    deq.pop_front();
                }
            }
        }
        dp = std::move(ndp);
    }

    int from_right = INF;
    for (int dest = N; dest; --dest) {
        dp[dest] = std::min(dp[dest], from_right + 1);
        from_right = std::min(from_right, dp[dest]);
    }

    for (int i = 1; i <= N; ++i) {
        std::cout << dp[i] << ' ';
    }
}

static void solve_set1() {
    int N;
    std::cin >> N;
    std::vector<int> permut(N);
    for (auto& n : permut) {
        std::cin >> n;
        --n;
    }

    // prepare reverse index
    std::vector<int> rev_ind(N);
    for (int i = 0; i < N; ++i)
        rev_ind[permut[i]] = i;

    // identify cycles
    std::vector<bool> visited(N);
    std::map<int, int> cycles_counts; // size -> number of occurences
    for (int i = 0; i < N; ++i) {
        if (visited[i])
            continue;
        visited[i] = true;
        int size{1};
        auto next = rev_ind[i];
        while (!visited[next]) {
            visited[next] = true;
            ++size;
            next = rev_ind[next];
        }
        ++cycles_counts[size];
    }

    std::vector<int> dp(N + 1, INF); // dp[i] cost of making i-cycle
    dp[0] = -1;
    for (const auto& [size, cnt] : cycles_counts) {
        for (int dest = N; dest; --dest)
            for (int full = 0; full <= cnt && full * size <= dest; ++full)
                dp[dest] = std::min(dp[dest], dp[dest - (full * size)] + full);
    }

    int from_right = INF;
    for (int dest = N; dest; --dest) {
        dp[dest] = std::min(dp[dest], from_right + 1);
        from_right = std::min(from_right, dp[dest]);
    }

    for (int i = 1; i <= N; ++i) {
        std::cout << dp[i] << ' ';
    }
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve_set1(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 level_design.cpp -o level_design.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address level_design.cpp -o level_design

Run:
level_design.exe < level_design.in

Input:

2
3
1 2 3
4
4 2 1 3

Output:

Case #1: 0 1 2 
Case #2: 0 1 0 1

*/

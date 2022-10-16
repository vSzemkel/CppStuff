
#include <iostream>
#include <numeric>
#include <vector>

// Happy Subarrays
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb2e1/0000000000c17491

static void solve() { // O(N)
    int N;
    std::cin >> N;
    std::vector<int> board(N);
    for (auto& v : board)
        std::cin >> v;

    std::vector<int64_t> prefsum(N + 1), prefsum2(N + 1);
    // partial sums of board: p[k] = b[0] + b[1] + .. + b[k]
    std::partial_sum(board.begin(), board.end(), prefsum.begin() + 1);
    // partial sum of initial prefixex: p2[k] = b[0..0] + b[0..1] + .. + b[0..k]
    std::partial_sum(prefsum.begin() + 1, prefsum.end(), prefsum2.begin() + 1);

    // next less element: for board[i] find minimal j: i < j && board[i] > board[j]
    std::vector<int64_t> stack, next(N + 1);
    for (int i = 0; i <= N; ++i) {
        while (!stack.empty() && prefsum[i] < prefsum[stack.back()]) {
            next[stack.back()] = i;
            stack.pop_back();
        }
        stack.push_back(i);
    }
    for (const int not_matched : stack)
        next[not_matched] = -1;

    int64_t ans{0};
    for (int l = 0; l < N; ++l) { // consider happy arrays starting at board[l]
        int r{N};
        if (~next[l])
            r = next[l] - 1;
        ans += prefsum2[r] - prefsum2[l] - (r - l) * prefsum[l];
    }

    std::cout << ans;
}

static void solve_set2() { // O(N2)
    int N;
    std::cin >> N;
    std::vector<int> board(N);
    for (auto& v : board)
        std::cin >> v;

    int64_t ans{0};
    for (int l = 0; l < N; ++l) {
        int64_t s{0};
        for (int r = l; r < N; ++r) {
            s += board[r];
            if (s >= 0)
                ans += s;
            else
                break;
        }
    }

    std::cout << ans;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 happy_subarrays.cpp -o happy_subarrays.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address happy_subarrays.cpp -o happy_subarrays

Run:
happy_subarrays.exe < happy_subarrays.in

Input:

3
5
1 -2 3 -2 4
3
1 0 3
7
1 1 1 1 1 1 1

Output:

Case #1: 14
Case #2: 12
Case #3: 84

*/

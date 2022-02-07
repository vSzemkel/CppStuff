/*
ID: marcin.8
LANG: C++
TASK: rockers
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=M8dKihAaeaL&S=rockers
*/

#include <fstream>
#include <vector>

std::ifstream task_in("rockers.in");
std::ofstream task_out("rockers.out");

int N, T, M; // num of songs, disk capacity, max number of disks
std::vector<short> items;

/**
 * @brief In this solution we suppose having an optimal solution 
 * for current config and trying to construct the extensions
 */
static void dp_solution() { // O(M * N * N)
    struct state_t {
        short total;      // total song released
        short last_time;  // last album time
    };

    // dp[a][s] holds optimal state for release not more then a albums with last released song s
    short ans{0};
    std::vector<std::vector<state_t>> dp(M + 1, std::vector<state_t>(N + 1, state_t{}));
    for (int a = 0; a < M; ++a)
        for (int s = 0; s <= N; ++s) {
            const auto& cur = dp[a][s]; // THIS IS COMPUTED, EXTEND
            ans = std::max(ans, cur.total);
            for (int n = s + 1; n <= N; ++n) {
                auto& next = dp[a][n];
                const int time = items[n - 1];
                if (time > T)
                    continue;
                if (cur.last_time + time <= T && (next.total < cur.total + 1 || (next.total == cur.total + 1 && cur.last_time + time < next.last_time))) {
                    next.total = cur.total + 1;
                    next.last_time = cur.last_time + time;
                } else if (dp[a + 1][n].total < cur.total + 1) {
                    dp[a + 1][n].total = cur.total + 1;
                    dp[a + 1][n].last_time = time;
                }
            }

        }

    task_out << ans << '\n';
}

static void full_search() { // O(2^N * N)
    int ans{0};
    const int SETS = 1 << N;
    for (int sel = 1; sel < SETS; ++sel) {
        int pos{0}, ret{0}, mask{1};
        for (int z = M; z; --z) {
            int sum{0};
            while (pos < N) {
                if (sel & mask) {
                    if (sum + items[pos] > T)
                        break;
                    sum += items[pos];
                    ++ret;
                }
                ++pos;
                mask <<= 1;
            }
        }
        if (ans < ret)
            ans = ret;
    }

    task_out << ans << '\n';
}

int main(int, char**)
{
    task_in >> N >> T >> M;
    items.resize(N);
    for (auto& s : items)
        task_in >> s;

    //full_search();
    dp_solution();
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 rockers.cpp -o rockers.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address rockers.cpp -o rockers

Input:

20 20 10 
18 15 16 10 2 20 14 17 3 7 16 15 18 16 20 16 13 9 4 16 
0b11110000001111111111
18 15 16 10 2 20 14 17 3 7 *16 *15 *18 *16 *20 *16 13 9 4 16 

Output:

14

*/

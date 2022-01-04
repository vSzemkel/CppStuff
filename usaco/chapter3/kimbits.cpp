/*
ID: marcin.8
LANG: C++
TASK: kimbits
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=L1O7vSeArjL&S=kimbits
*/

#include <fstream>
#include <vector>

std::ifstream task_in("kimbits.in");
std::ofstream task_out("kimbits.out");

static auto pascal(const int n) {
    std::vector<std::vector<int64_t>> ret(n + 1, std::vector<int64_t>(n + 1));
    for (int r = 0; r <= n; ++r) {
        ret[r][0] = ret[r][r] = 1;
        for (int c = 1; c < r; ++c)
            ret[r][c] = ret[r - 1][c - 1] + ret[r - 1][c];
    }

    return ret;
}

std::vector<std::vector<int64_t>> bc;

/**
 * @brief Number of subsets of N elements with subset size at most K
 */
static int64_t all(const int N, const int K) {
    int64_t ret{1};
    for (int i = 1; i <= K; ++i)
        ret += bc[N][i];
    return ret;
}

static void solve(const int N, const int L, const int64_t I) {
    if (N == 1) {
        task_out << (I == 0 ? '0' : '1') << '\n';
        return;
    }

    const auto sp = all(N - 1, L);
    if (I < sp) {
        task_out << '0';
        solve(N - 1, L, I);
    } else {
        task_out << '1';
        solve(N - 1, L - 1, I - sp);
    }
}

int main(int, char**)
{
    int N, L;
    int64_t I;
    task_in >> N >> L >> I;
    bc = pascal(N);
    solve(N, L, --I);
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 kimbits.cpp -o kimbits.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address kimbits.cpp -o kimbits

Input:

5 3 19

Output:

10011

*/

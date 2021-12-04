/*
ID: marcin.8
LANG: C++
TASK: nocows
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=wHmtxK0SauG&S=nocows
*/

#include <iostream>
#include <fstream>
#include <vector>

std::ifstream task_in("nocows.in");
std::ofstream task_out("nocows.out");

const int M = 9901;
std::vector<std::vector<int>> dp; // dp[k][n] :: -1 impossible, 0 not known yet
std::vector<int> min_tree, max_tree;

static int combinations(const int n, const int k) {
    auto& result = dp[k][n];
    if (result)
        return result > 0 ? result : 0;

    const auto mi = min_tree[k - 1];
    const auto ma = max_tree[k - 1];
    for (int left = mi, right = n - 1 - left; left <= n && left <= ma && right >= 1; left += 2, right -= 2) {
        const int full_heigth_subtree = combinations(left, k - 1);
        for (int h = 1; h < k - 1; ++h)
            result = (result + (2 * full_heigth_subtree * combinations(right, h) % M)) % M;
        result = (result + (full_heigth_subtree * combinations(right, k - 1) % M)) % M;
    }

    return result;
}

static void init(const int N, const int K) {
    dp.assign(K + 1, std::vector<int>(N + 1, -1));
    min_tree.assign(K + 1, 0);
    max_tree.assign(K + 1, 0);
    for (int h = 1; h <= K; ++h) {
        auto& t = dp[h];
        const int mi = min_tree[h] = 2 * h - 1;
        const int ma = max_tree[h] = std::min((2 * max_tree[h - 1] + 1), N + 1); // tricky part
        for (int c = 1; c <= N; c += 2) {
            if (mi > N)
                break;
            for (int i = mi; i <= N && i < ma; i += 2)
                t[i] = 0;
            if (ma <= N)
                t[ma] = 1;
        }
    }
}

int main(int, char**)
{
    int N, K; task_in >> N >> K;
    init(N, K);
    task_out << combinations(N, K) << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 nocows.cpp -o nocows.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address nocows.cpp -o nocows

Input:

165 65

Output:

3470

*/

/*
ID: marcin.8
LANG: C++
TASK: ariprog
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?S=ariprog&a=JG8ONJjSzJv
*/

#include <algorithm>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

std::ifstream task_in("ariprog.in");
std::ofstream task_out("ariprog.out");

using seq_t = std::pair<int, int>; // {b, a}

std::vector<int> target;
std::unordered_set<int> checker;

static std::vector<seq_t> solve(const int N) { // O(M^2*log(N))
    const int max = int(target.back());
    const int size = int(target.size());
    std::vector<seq_t> ret; // {b, a}
    for (int i = 0; i < size; ++i)
        for (int j = i + 1; j < size; ++j) {
            const int a{target[i]}, b{target[j] - a};
            const int e = a + (N - 1) * b;
            if (e > max)
                break;
            int len{2};
            while (len < N) {
                const int c = a + (N - len + 1) * b;
                if (checker.count(c) == 0)
                    break;
                ++len;
            }
            if (len == N)
                ret.emplace_back(b, a);
        }

    return ret;
}

static std::vector<seq_t> overcomplicated(const int N) { // O(M^2*log(N))
    using qelem_t = std::pair<int, seq_t>; // {n, seq}
    std::unordered_map<int, std::unordered_map<int, int>> memo; // memo[a][b] == n
    std::priority_queue<qelem_t, std::vector<qelem_t>, std::greater<>> pq;

    const int max = int(target.back());
    const int size = int(target.size());
    for (int i = 0; i < size; ++i)
        for (int j = i + 1; j < size; ++j) {
            const int a{target[i]}, b{target[j] - a};
            const int c = a + 2 * b;
            if (c > max)
                break;
            if (checker.count(c)) {
                memo[a][b] = 3;
                pq.emplace(3, seq_t{b, a});
            } else
                memo[a][b] = 2;
        }

    while (!pq.empty()) {
        const auto item = pq.top();
        const int len = item.first;
        if (len == N) break;
        const int a = item.second.second;
        const int b = item.second.first;
        const int e = a + (len - 1) * b;
        if (memo[e].count(b) > 0) {
            const int new_len = len + memo[e][b] - 1;
            memo[a][b] = new_len;
            pq.emplace(new_len, seq_t{b, a});
        }
        pq.pop();
    }

    std::vector<seq_t> ret; // {b, a}
    while (!pq.empty()) {
        const auto item = pq.top(); pq.pop();
        ret.push_back(item.second);
    }

    return ret;
}

int main(int, char**)
{
    int N, M; task_in >> N >> M;
    // generate p*p + q*q set
    for (int p = 0; p <= M; ++p)
        for (int q = p; q <= M; ++q)
            checker.insert(p * p + q * q);
    target.assign(checker.begin(), checker.end());
    std::sort(target.begin(), target.end());

    // auto ret = overcomplicated(N);
    auto ret = solve(N);

    if (ret.empty())
        task_out << "NONE\n";
    else {
        std::sort(ret.begin(), ret.end());
        for (const auto& [b, a] : ret)
            task_out << a << ' ' << b << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 ariprog.cpp -o ariprog.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address ariprog.cpp -o ariprog

Input:

21 200

Output:

1217 84
2434 168
4868 336
6085 420
9736 672
10953 756
12170 840
12953 924
15821 1092

*/

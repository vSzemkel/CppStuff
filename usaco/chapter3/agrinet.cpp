/*
ID: marcin.8
LANG: C++
TASK: agrinet
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=J7XcsxGgQb7&S=agrinet
*/

#include <array>
#include <fstream>
#include <queue>
#include <vector>

std::ifstream task_in("agrinet.in");
std::ofstream task_out("agrinet.out");

int main(int, char**)
{
    int N; task_in >> N;
    std::vector<std::vector<int>> dist(N, std::vector<int>(N));
    for (auto& row : dist)
        for (auto& d : row)
            task_in >> d;

    int ret{0};
    std::vector<bool> seen(N);
    std::priority_queue<std::array<int, 2>, std::vector<std::array<int, 2>>, std::greater<>> pq;
    pq.push({0, 0}); // {dist, to}
    while (!pq.empty()) {
        const auto edge = pq.top(); pq.pop();
        if (!seen[edge[1]]) {
            seen[edge[1]] = true;
            ret += edge[0];
            for (int n = 0; n < N; ++n)
                if (n != edge[1] && !seen[n])
                    pq.push({dist[edge[1]][n], n});
        }
    }

    task_out << ret << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 agrinet.cpp -o agrinet.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address agrinet.cpp -o agrinet

Input:

4
0 4 9 21
4 0 8 17
9 8 0 16
21 17 16 0

Output:

28

*/

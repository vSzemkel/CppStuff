/*
ID: marcin.8
LANG: C++
TASK: job
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=BxiyJvhIDID&S=job
*/

#include <algorithm>
#include <fstream>
#include <numeric>
#include <queue>
#include <vector>

std::ifstream task_in("job.in");
std::ofstream task_out("job.out");

/**
 * @brief M machines produce N items
 * @return delivery times
 */
auto times(const int M, const int N) {
    // {completion, machine latency}
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
    std::vector<int> times(N);

    for (int i = 0; i < M; ++i) {
        int t; task_in >> t;
        pq.push({t, t});
    }
    for (int z = N - 1; z; --z) {
        auto p = pq.top(); pq.pop();
        times[z] = p.first;
        p.first += p.second;
        pq.push(p);
    }

    times[0] = pq.top().first;
    return times;
}

int main(int, char**)
{
    int N, A, B;
    task_in >> N >> A >> B;

    std::vector<int> completionA = times(A, N);
    std::vector<int> completionB = times(B, N);

    std::transform(completionB.cbegin(), completionB.cend(), completionA.crbegin(), completionB.begin(), std::plus<>{});
    const auto ans = *std::max_element(completionB.cbegin(), completionB.cend());
    task_out << completionA[0] << ' ' << ans << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 job.cpp -o job.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address job.cpp -o job

Input:

2 2 2 
3 5 
8 5

Output:

5 11

*/

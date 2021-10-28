
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <set>
#include <unordered_map>
#include <vector>

// Graph travel
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000435bae/0000000000888764


constexpr int bits(int x) { return x == 0 ? 0 : 31 - __builtin_clz(x); } // floor(log2(x)) 

int N;
std::vector<int64_t> L, R, A; // {L, R, A}

static void solve() { // by Benq
    int M; int64_t K;
    std::cin >> N >> M >> K;
    L.resize(N); R.resize(N); A.resize(N);
    for (int i = 0; i < N; ++i)
        std::cin >> L[i] >> R[i] >> A[i];

    std::vector<int> edge_mask(N); // i-th node represented as 2**i
    for (int z = M; z; --z) {
        int r1, r2; std::cin >> r1 >> r2;
        edge_mask[r1] |= 1 << r2;
        edge_mask[r2] |= 1 << r1;
    }

    const int MAX = 1 << N;
    std::vector<int64_t> sums(MAX); // path_count of nodes subset
    std::vector<int64_t> solutions(MAX); // path_count of nodes subset
    for (int m = 1, hi = 0; m < MAX; ++m) {
        const int pred = m & (m - 1); // m with lowest bit cleared
        if (pred == 0) {
            sums[m] = A[hi++];
            solutions[m] = 1;
        } else 
            sums[m] = sums[pred] + A[bits(m & -m)];
    }
    
    int64_t ans{0};
    for (int m = 1; m < MAX; ++m) { // for every subset
        int all_adj{0};
        for (int nodes = m; nodes; ) {
            const int n = nodes - (nodes & (nodes - 1));
            all_adj |= edge_mask[bits(n)];
            nodes -= n;
        }
        const auto sum = sums[m];
        if (sum == K)
            ans += solutions[m];
        // try to expand it with any single node
        for (int ex = 0; ex < N; ++ex) {
            const int node = 1 << ex;
            if (!(m & node) && (all_adj & node) && L[ex] <= sum && sum <= R[ex])
                solutions[m | node] += solutions[m];
        }
    }

    std::cout << ans;
}

std::vector<int> path;
std::vector<bool> visited;
std::vector<std::vector<int>> edges;
std::set<std::vector<int>> solutions;

static void compute_set1(const int64_t accu, const int64_t target) {
    if (target == 0)
        solutions.insert(path);
    if (target <= 0)
        return;

    for (int i = 0; i < int(path.size()); ++i) // this approach skips any backward paths
        for (const auto& next : edges[path[i]]) {
            if (!visited[next] && L[next] <= accu && accu <= R[next]) {
                const auto delta = A[next];
                visited[next] = true;
                path.push_back(next);

                compute_set1(accu + delta, target - delta);
                visited[next] = false;
                path.pop_back();
            }
        }
}

// Problem with this approach is considering backward paths. It is too costly.
static void correct_naive_tle(int start, int64_t accu, int64_t target) {
    int depth; // to compile
    if (target == 0)
        solutions.insert(path);
    if (target <= 0)
        return;

    const auto path_copy = path;
    const auto depth_copy = depth;
    const auto visited_copy = visited;
    for (const auto& next : edges[start]) 
        if (visited[next] || (L[next] <= accu && accu <= R[next])) {
            int64_t delta{0};
            if (!visited[next]) {
                visited[next] = true;
                path.push_back(next);
                delta = A[next];
            }

            if (depth < N) {
                ++depth;
                correct_naive_tle(next, accu + delta, target - delta);
                path = path_copy;
                depth = depth_copy;
                visited = visited_copy;
            }
        }
}

static void solve_set1() {
    int M; int64_t K;
    std::cin >> N >> M >> K;
    path.reserve(N); 
    L.resize(N); R.resize(N); A.resize(N);
    for (int i = 0; i < N; ++i)
        std::cin >> L[i] >> R[i] >> A[i];
    edges.assign(N, std::vector<int>{});
    for (int z = M; z; --z) {
        int r1, r2; std::cin >> r1 >> r2;
        edges[r1].push_back(r2);
        edges[r2].push_back(r1);
    }
    solutions.clear();

    for (int r = 0; r < N; ++r) {
        path.assign(1, r);
        visited.assign(N, false);
        visited[r] = true;
        const auto& delta = A[r];
        compute_set1(delta, K - delta); 
    }

    std::cout << solutions.size();
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << std::endl;
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 graph_travel.cpp -o graph_travel.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address graph_travel.cpp -o graph_travel

Run:
graph_travel.exe < graph_travel.in

Input:

3
4 3 3
1 3 1
1 1 1
2 4 1
2 3 1
0 1
1 2
2 3
4 5 3
1 3 1
1 1 1
2 4 1
2 3 1
0 1
1 2
2 3
3 0
0 2
4 1 2
0 4 1
0 4 1
0 4 2
0 4 2
0 1

Output:


*/

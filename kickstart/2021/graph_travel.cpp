
#include <algorithm>
#include <array>
#include <iostream>
#include <set>
#include <vector>

// Graph travel
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000435bae/0000000000888764


int N;
std::vector<int> path;
std::vector<bool> visited;
std::vector<std::vector<int>> edges;
std::set<std::vector<int>> solutions;
std::vector<std::array<int64_t, 3>> rooms; // {L, R, A}

static void compute_set1(const int64_t accu, const int64_t target) {
    if (target == 0)
        solutions.insert(path);
    if (target <= 0)
        return;

    for (int i = 0; i < int(path.size()); ++i) // this approach skips any backward paths
        for (const auto& next : edges[path[i]]) {
            const auto& room = rooms[next];
            if (!visited[next] && room[0] <= accu && accu <= room[1]) {
                const auto delta = room[2];
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
        if (visited[next] || (rooms[next][0] <= accu && accu <= rooms[next][1])) {
            int64_t delta{0};
            if (!visited[next]) {
                visited[next] = true;
                path.push_back(next);
                delta = rooms[next][2];
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
    rooms.resize(N); // {L, R, A}
    for (auto& r : rooms)
        std::cin >> r[0] >> r[1] >> r[2];
    edges.assign(N, std::vector<int>{});
    for (int z = M; z; --z) {
        int r1, r2; std::cin >> r1 >> r2;
        edges[r1].push_back(r2);
        edges[r2].push_back(r1);
    }
    solutions.clear();

    std::vector<bool> visited_anytime(N);
    for (int r = 0; r < N; ++r) {
        if (visited_anytime[r])
            continue;
        path.assign(1, r);
        visited.assign(N, false);
        visited[r] = true;
        const auto& delta = rooms[r][2];
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
        std::cout << "Case #" << g << ": "; solve_set1(); std::cout << std::endl;
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

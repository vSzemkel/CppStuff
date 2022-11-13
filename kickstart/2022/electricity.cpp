
#include <iostream>
#include <vector>

// Electricity
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb1b6/0000000000c47c8e

std::vector<std::vector<int>> adj;
std::vector<int> weight, stack;
std::vector<bool> seen;

static int spread(const int n) {
    int ret{0};
    stack.assign(1, n);
    while (!stack.empty()) {
        const int cur = stack.back();
        stack.pop_back();
        seen[cur] = true;
        ++ret;
        for (const int next : adj[cur])
            if (weight[next] < weight[cur])
                stack.push_back(next);
    }

    return ret;
}

static void solve() {
    int N;
    std::cin >> N;
    seen.assign(N, false);
    weight.resize(N);
    for (auto& w : weight)
        std::cin >> w;
    adj.assign(N, {});
    for (int z = N - 1; z; --z) {
        int s, d;
        std::cin >> s >> d;
        --s; --d;
        adj[s].push_back(d);
        adj[d].push_back(s);
    }

    int ans{0};
    for (int i = 0; i < N; ++i)
        if (!seen[i])
           ans = std::max(ans, spread(i));

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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 electricity.cpp -o electricity.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address electricity.cpp -o electricity

Run:
electricity.exe < electricity.in

Input:

2
5
1 2 3 4 3
1 3
2 3
4 3
4 5
6
1 2 3 3 1 4
3 1
3 2
3 4
4 5
1 6

Output:

Case #1: 5
Case #2: 3

*/


#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

// Chain Reactions
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000876ff1/0000000000a45ef7

template <class Fun>
class y_combinator_result
{
    Fun fun_;

  public:
    template <class T>
    explicit y_combinator_result(T&& fun) : fun_(std::forward<T>(fun)) {}

    template <class... Args>
    decltype(auto) operator()(Args&&... args)
    {
        return fun_(std::ref(*this), std::forward<Args>(args)...);
    }
};

template <class Fun>
decltype(auto) y_combinator(Fun&& fun)
{
    return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

static void ecnerwala() {
    int N; std::cin >> N;
    std::vector<int64_t> F(N);
    for (auto& f : F)
        std::cin >> f;
    std::vector<int> P(N);
    for (auto& p : P) {
        std::cin >> p;
        p--;
    }

    const int64_t INF = 1e18;
    int64_t ans = 0;
    std::vector<int64_t> ch_min(N, INF);
    for (int i = N - 1; i >= 0; i--) {
        int64_t v = F[i];
        if (ch_min[i] != INF) {
            ans -= ch_min[i];
            v = std::max(v, ch_min[i]);
        }
        ans += v;
        if (P[i] != -1) {
            ch_min[P[i]] = std::min(ch_min[P[i]], v);
        }
    }

    std::cout << ans;
}

static void solve() { // by ksun48
    int N; std::cin >> N;
    std::vector<int> funfacs(N + 1);
    funfacs[0] = 0;
    for (int i = 1; i <= N; ++i)
        std::cin >> funfacs[i];
    std::vector<int> prev(N + 1);
    prev[0] = -1;
    for (int i = 1; i <= N; ++i)
        std::cin >> prev[i];

    std::vector<std::vector<int>> adj(N + 1);
    for (int i = 1; i <= N; ++i)
        adj[prev[i]].push_back(i);

    // lambda returns max element on optimal path to argument node
    int64_t side_trees{0};
    int root_path = y_combinator(
        [&](auto self, int n) -> int {
            if (adj[n].empty())
                return funfacs[n];
            std::vector<int> subtrees;
            for (const int s : adj[n])
                subtrees.push_back(self(s));
            const int m = *std::min_element(subtrees.begin(), subtrees.end());
            side_trees += std::accumulate(subtrees.begin(), subtrees.end(), 0LL);
            side_trees -= m;
            return std::max(funfacs[n], m);
        }
    )(0);

    std::cout << root_path + side_trees;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; ecnerwala(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 chain_reactions.cpp -o chain_reactions.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address chain_reactions.cpp -o chain_reactions

Run:
chain_reactions.exe < chain_reactions.in

Input:

3
4
60 20 40 50
0 1 1 2
5
3 2 1 4 5
0 1 1 1 0
8
100 100 100 90 80 100 90 100
0 1 2 1 2 3 1 3

Output:

Case #1: 110
Case #2: 14
Case #3: 490

*/

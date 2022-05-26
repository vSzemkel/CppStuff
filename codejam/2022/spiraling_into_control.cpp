
#include <algorithm>
#include <iostream>
#include <queue>
#include <map>
#include <unordered_set>
#include <utility>
#include <vector>

// Spiraling Into Control
// https://codingcompetitions.withgoogle.com/codejam/round/00000000008778ec/0000000000b15a74

static void solve() { // ecnerwala
    int N, K;
    std::cin >> N >> K;

    if (K < N - 1 || K & 1) {
        std::cout << "IMPOSSIBLE\n";
        return;
    }

    const int N2 = N * N;
    int len{N + 2}, step{0}, skip, b, e; // b->e is a shortcut from ring0->ring1
    const auto advance = [&](){
        if (step % 4 == 0) {
            len -= 2;
            b = N2 - len * len + 2;
            e = b + 4 * (len - 1) - 1;
        } else {
            b += len - 1;
            e += len - 3;
        }
        skip = e - b - 1;
        ++step;
        // std::cerr << b << "->" << e << "skip: " << skip << '\n';
    };

    advance(); // init
    std::vector<std::pair<int, int>> ans;
    for (int to_skip = N2 - 1 - K; to_skip; ) {
        if (skip <= to_skip) {
            to_skip -= skip;
            ans.emplace_back(b, e);
            advance();
            advance();
            advance();
        }
        advance();
    }

    std::cout << ans.size() << '\n';
    for (const auto& [b, e] : ans)
        std::cout << b << ' ' << e << '\n';
}

struct hist_t {
    int node;
    int dist;
    std::vector<int> cuts;
};

static void solve_set2() {
    int N, K;
    std::cin >> N >> K;

    if (K < N - 1 || K & 1) {
        std::cout << "IMPOSSIBLE\n";
        return;
    }

    const int N2 = N * N;
    std::map<int, int, std::greater<>> shortcuts;
    for (int i = 3; i <= N; i += 2) {
        const int start = i * i - 1;
        const int stride = i - 1;
        const int diff = start - (i - 2) * (i - 2);
        for (int i = 0; i < 4; ++i)
            shortcuts[start - i * stride] = diff - 2 * i;
    }

    std::queue<hist_t> qq;
    std::unordered_set<int64_t> seen;
    const auto push = [&](const hist_t& h) { // duplicates filtering out
        const auto can = (int64_t(h.node) << 32) + h.dist;
        if (seen.count(can))
            return;
        seen.insert(can);
        qq.push(h);
    };

    hist_t ans{N2 - 1, 1, {}};
    push(ans);
    while (!qq.empty()) {
        hist_t& cur = qq.front();
        if (cur.dist >= K || cur.node <= 1) {
            if (cur.dist == K && cur.node == 1) {
                ans = cur;
                break;
            }
        } else {
            auto f = shortcuts.lower_bound(cur.node);
            if (f != shortcuts.end()) {
                // use shortcut
                auto sc = cur;
                sc.dist += sc.node - f->first + 1;
                sc.node = f->first - f->second;
                sc.cuts.push_back(f->first);
                if (sc.dist <= K && K - sc.dist < sc.node)
                    push(sc);
                // step into next
                f = std::next(f);
                if (f != shortcuts.end()) {
                    cur.dist += cur.node - f->first;
                    cur.node = f->first;
                    if (cur.dist <= K && K - cur.dist < cur.node)
                        push(cur);
                }
            } else {
                cur.dist += cur.node - 1;
                cur.node = 1;
                push(cur);
            }
        }
        qq.pop();
    }

    if (ans.dist == 1)
        std::cout << "IMPOSSIBLE\n";
    else { // !ans.cuts.empty() as K < N2 - 1
        if (ans.cuts.back() == 2)
            ans.cuts.pop_back();
        std::cout << ans.cuts.size() << '\n';
        for (const auto& c : ans.cuts)
            std::cout << N2 - c + 1 << ' ' << N2 - (c - shortcuts[c]) + 1 << '\n';
    }
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve();
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 spiraling_into_control.cpp -o spiraling_into_control.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address spiraling_into_control.cpp -o spiraling_into_control

Run:
spiraling_into_control.exe < spiraling_into_control.in

Input:

4
5 4
5 3
5 12
3 1

Output:

Case #1: 2
2 17
18 25
Case #2: IMPOSSIBLE
Case #3: 2
10 21
22 25
Case #4: IMPOSSIBLE

*/

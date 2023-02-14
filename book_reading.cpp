
#include <deque>
#include <iostream>
#include <utility>
#include <vector>

// Book Reading
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050e02/000000000018fd0d

static void solve() { // O(NlogN + Q)
    int N, M, Q;
    std::cin >> N >> M >> Q;
    std::vector<bool> torn_outs(N + 1);
    for (int z = M; z; --z) {
        int p; std::cin >> p;
        torn_outs[p] = true;
    }

    std::vector<int> memo(N + 1); // memo[x] is number of pages that are multiples of x and not torn out
    for (int i = 1; i <= N; ++i)
        for (int j = i; j <= N; j += i)
            if (!torn_outs[j])
                ++memo[i];

    int64_t ans{};
    for (int z = Q; z; --z) {
        int r; std::cin >> r;
        ans += memo[r];
    }

    std::cout << ans;
}

static void solve_set1() { // O(N * Q)
    int N, M, Q;
    std::cin >> N >> M >> Q;
    std::vector<bool> torn_outs(N + 1);
    for (int z = M; z; --z) {
        int p; std::cin >> p;
        torn_outs[p] = true;
    }
    using page_t = std::pair<int64_t, int>;
    std::deque<page_t> pages;
    for (int z = Q; z; --z) {
        int r; std::cin >> r;
        if (r <= N)
            pages.emplace_back(r, r);
    }

    int ans{};
    while (!pages.empty()) {
        const auto [page, base] = pages.front();
        pages.pop_front();
        if (!torn_outs[page])
            ++ans;

        const auto new_page = page + base;
        if (new_page <= N)
            pages.emplace_back(new_page, base);
    }

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
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++17 book_reading.cpp -o book_reading.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address book_reading.cpp -o book_reading

Run:
book_reading.exe < book_reading.in

Input:

3
11 1 2
8
2 3
11 11 11
1 2 3 4 5 6 7 8 9 10 11
1 2 3 4 5 6 7 8 9 10 11
1000 6 1
4 8 15 16 23 42
1

Output:

Case #1: 7
Case #2: 0
Case #3: 994  

*/

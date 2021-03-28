
#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

// Reversort & Reversort Engineering
// https://codingcompetitions.withgoogle.com/codejam/round/000000000043580a/00000000006d0a5c
// https://codingcompetitions.withgoogle.com/codejam/round/000000000043580a/00000000006d12d7


static int compute_cost(std::vector<int> data) {
    int ret{0};
    for (int i = 0; i < (int)data.size() - 1; ++i) {
        const auto it = std::min_element(data.begin() + i, data.end());
        std::reverse(data.begin() + i, it + 1);
        ret += it - (data.begin() + i) + 1;
    }

    return ret;
}

static void solve() {
    int N, C; std::cin >> N >> C;
    if (C < N - 1 || N * (N + 1) / 2 - 1 < C) {
        std::cout << " IMPOSSIBLE";
        return;
    }

    int next{1};
    int left{0};
    int right{N - 1};
    bool last_left{true};
    const int orgC{C};

    C -= N - 1;
    std::vector<int> data(N);
    while (left < right) {
        if (right - left <= C) {
            last_left = !last_left;
            C -= right - left;
        }
        if (last_left) data[left++] = next;
        else data[right--] = next;
        ++next;
    }
    data[left] = next;

    assert(compute_cost(data) == orgC);

    for (const int n : data)
        std::cout << " " << n;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ":"; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 reversort_engineering.cpp -o reversort_engineering.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address reversort_engineering.cpp -o reversort_engineering.o

Run:
reversort_engineering.exe < reversort_engineering.in

Input:

13
7 5
7 6
7 7
7 8
7 9
7 10
7 11
7 12
7 13
7 23
7 28
4 6
2 1

Output:

Case #1: IMPOSSIBLE
Case #2: 1 2 3 4 5 6 7
Case #3: 1 2 3 4 5 7 6
Case #4: 1 2 3 4 7 6 5
Case #5: 1 2 3 7 6 5 4
Case #6: 1 2 7 6 5 4 3
Case #7: 1 7 6 5 4 3 2
Case #8: 7 6 5 4 3 2 1
Case #9: 6 7 5 4 3 2 1
Case #10: 2 5 6 7 4 3 1
Case #11: IMPOSSIBLE
Case #12: 4 3 2 1
Case #13: 1 2

*/

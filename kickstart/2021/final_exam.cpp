
#include <iostream>
#include <set>
#include <utility>

// Final Exam
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000004361e3/000000000082bffc#problem


static void solve() {
    int N, M; std::cin >> N >> M;
    std::set<std::pair<int64_t, int64_t>> segments; // {begin, end}
    for (int i = 0; i < N; ++i) {
        int64_t b, e; std::cin >> b >> e;
        segments.emplace(b, e);
    }

    for (int i = 0; i < M; ++i) {
        int64_t s; std::cin >> s;
        auto it = std::lower_bound(segments.begin(), segments.end(), std::pair{s, s});
        if (it == segments.begin()) {
            const int64_t b{it->first}, e{it->second};
            if (b < e)
                segments.emplace(b + 1, e);
            s = b;
        } else {
            const int64_t n = (it == segments.end()) ? -1 : it->first;
            --it;
            int64_t b{it->first}, e{it->second};
            if (b <= s && s <= e) {
                if (b < s) 
                    segments.emplace(b, s - 1);
                if (s < e) 
                    segments.emplace(s + 1, e);
            } else { // e < s < n
                if (n == -1 || s - e <= n - s) {
                    if (b < e)
                        segments.emplace(b, e - 1);
                    s = e;
                } else {
                    ++it;
                    e = it->second;
                    if (n < e)
                        segments.emplace(n + 1, e);
                    s = n;
                }
            }
        }

        segments.erase(it);
        std::cout << ' ' << s;
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
        std::cout << "Case #" << g << ":"; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 final_exam.cpp -o final_exam.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address final_exam.cpp -o final_exam

Run:
final_exam.exe < final_exam.in

Input:

2
5 4
1 2
6 7
9 12
24 24
41 50
14 24 24 4
1 1
42 42
24

Output:

Case #1: 12 24 11 2
Case #2: 42

*/

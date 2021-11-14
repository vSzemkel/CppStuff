
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

// Silly substitutions
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000435914/00000000008d94f5


static void solve() {
    int N; std::string S;
    std::cin >> N >> S;

    constexpr auto nc = [](const char d) -> char { return d == '9' ? '0' : d + 1; };
    // need single linked list
    std::vector<int> pred(N), succ(N);
    std::iota(succ.begin(), succ.end(), 1);
    std::iota(pred.begin(), pred.end(), -1);
    // need to remember positions of interesting tokens
    int total{0};
    std::vector<int> map[10];
    for (int i = 0; i < N - 1; ++i)
        if (nc(S[i]) == S[i + 1]) {
            map[S[i] - '0'].push_back(i);
            ++total;
        }
    // eliminate
    int pos{0};
    std::vector<bool> del(N);
    while (total) {
        auto& m = map[pos];
        while (!m.empty()) {
            const int i = m.back(); m.pop_back();
            const int prev = pred[i];
            const int next = succ[i];
            --total;
            if (S[i] - '0' != pos || S[next] != nc(S[i])) // coalesced earlier
                continue;
            const char val = nc(S[next]);
            del[i] = true;
            // if (succ[i] < N) - from invariant
            pred[next] = prev;
            if (prev >= 0)
                succ[prev] = next;
            S[next] = val;

            if (prev >= 0 && nc(S[prev]) == val) {
                map[S[prev] - '0'].push_back(prev);
                //map[(pos + 1) % 10].push_back(prev);
                ++total;
            }
            if (succ[next] < N && nc(val) == S[succ[next]]) {
                map[val - '0'].push_back(next);
                //map[(pos + 2) % 10].push_back(prev);
                ++total;
            }
        }
        pos = (pos + 1) % 10;
    }

    for (int i = 0; i < N; ++i)
        if (!del[i]) std::cout << S[i];
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 silly_substitutions.cpp -o silly_substitutions.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address silly_substitutions.cpp -o silly_substitutions

Run:
silly_substitutions.exe < silly_substitutions.in

Input:

8
2
54
3
012
4
0145
5
00000
11
98765432101
5
65601
5
43456
5
10901

Output:

Case #1: 54
Case #2: 22
Case #3: 26
Case #4: 00000
Case #5: 1
Case #6: 82
Case #7: 8
Case #8: 1092

*/

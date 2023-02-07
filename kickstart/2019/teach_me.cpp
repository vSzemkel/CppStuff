
#include <iostream>
#include <map>
#include <set>
#include <unordered_set>
#include <vector>

// Teach Me
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050edc/00000000001864bc

static void solve() {
    int64_t N, S;
    std::cin >> N >> S;
    std::vector<std::set<int>> people(N); // person -> set of skills
    std::map<std::set<int>, int64_t> counts; // skill set -> number of mentors with this set
    for (auto& skills : people) {
        int c; std::cin >> c;
        for (int y = c; y; --y) {
            int s; std::cin >> s;
            skills.insert(s);
        }
        ++counts[skills];
    }

    int64_t ans = N * N;
    for (const auto& skills : people) {
        const int ub = 1 << skills.size();
        for (int mask = 0; mask < ub; ++mask) {
            int check{1};
            std::set<int> subset;
            for (const auto s : skills) {
                if (mask & check)
                    subset.insert(s);
                check <<= 1;
            }
            ans -= counts[subset];
        }
    }

    std::cout << ans;
}

static void solve_set1() {
    int N, S;
    std::cin >> N >> S;
    std::vector<std::unordered_set<int>> skills(N); // skill -> number of teachers
    for (auto& t : skills) {
        int c; std::cin >> c;
        for (int y = c; y; --y) {
            int s; std::cin >> s;
            t.insert(s);
        }
    }

    int64_t ans{};
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (i != j)
                for (const auto s : skills[i])
                    if (skills[j].count(s) == 0) {
                        ++ans;
                        break;
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
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++17 teach_me.cpp -o teach_me.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address teach_me.cpp -o teach_me

Run:
teach_me.exe < teach_me.in

Input:

2
4 100
4 80 90 100 5
1 90
1 80
3 80 90 100
3 30
4 10 11 12 13
4 10 11 12 13
5 25 26 27 28 29

Output:

Case #1: 7
Case #2: 4

*/

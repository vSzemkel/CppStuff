
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

// Students and Mentors
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb0f5/0000000000ba84ae

static void solve() {
    int N;
    std::cin >> N;
    std::vector<int> students(N);
    for (auto& s : students)
        std::cin >> s;

    auto rank = students;
    std::sort(rank.begin(), rank.end());
    std::unordered_map<int, int> rev_rank; // student[i] -> rank
    for (int i = 0; i < N; ++i)
        rev_rank[rank[i]] = i;

    for (int i = 0; i < N; ++i) {
        const int base = 2 * students[i] + 1;
        auto it = std::lower_bound(rank.begin(), rank.end(), base);
        if (it == rank.begin())
            std::cout << "-1 ";
        else {
            --it;
            const auto can = it - rank.begin();
            if (can == rev_rank[students[i]]) {
                if (it == rank.begin())
                    std::cout << "-1 ";
                else {
                    --it;
                    std::cout << *it << ' ';
                }
            } else
                std::cout << *it << ' ';
        }
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
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 students_and_mentors.cpp -o students_and_mentors.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address students_and_mentors.cpp -o students_and_mentors

Run:
students_and_mentors.exe < students_and_mentors.in

Input:

3
3
2000 1500 1900
5
1000 600 1000 2300 1800
2
2500 1200

Output:

Case #1: 1900 2000 2000 
Case #2: 1800 1000 1800 1800 2300
Case #3: 1200 -1

*/

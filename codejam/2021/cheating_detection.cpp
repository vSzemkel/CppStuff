
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

// Cheating Detection
// https://codingcompetitions.withgoogle.com/codejam/round/000000000043580a/00000000006d1155

constexpr int64_t g_rows = 100;
constexpr int64_t g_cols = 10000;

static void solve() { // by ksun48
    std::vector<std::string> data(g_rows);
    for (auto& s : data)
        std::cin >> s;

    std::vector<std::pair<int, int>> question_solves(g_cols); // {rank, question}
    for (int c = 0; c < g_cols; ++c) {
        question_solves[c].second = c;
        for (int r = 0; r < g_rows; ++r)
            if (data[r][c] == '1')
                ++question_solves[c].first;
    }

    std::sort(question_solves.begin(), question_solves.end(), [](const auto& q1, const auto& q2){
        return q1.first > q2.first;
    });

    std::vector<double> scores(g_rows);
    for (int p = 0; p < g_rows; ++p) {
        int hits{0};
        int misses{0};
        int factor{0};
        for (int q = 0; q < g_cols; ++q) {
            if (data[p][question_solves[q].second] == '1') {
                ++hits;
                factor += misses;
            } else
                ++misses;
        }
        double f = factor;
        f /= hits * misses;
        scores[p] = f;
    }

    const auto cheater = std::max_element(scores.begin(), scores.end());
    std::cout << cheater - scores.begin() + 1;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases, P;
    std::cin >> no_of_cases >> P;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 cheating_detection.cpp -o cheating_detection.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address cheating_detection.cpp -o cheating_detection.o

Run:
cheating_detection.exe < cheating_detection.in

*/

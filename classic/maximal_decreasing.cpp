
#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>

// Maximal decreasing sequence
// Given an integer sequence find the length of maximal decreasing non-consecutive subsequence

const auto rand_in_range = [](const int ubound){ std::random_device seed; std::mt19937 gen{seed()}; std::uniform_int_distribution<int> dist(0, ubound - 1); return dist(gen); };

template <typename C>
static void print(const C& v)
{
    if (v.empty()) return;
    std::cout << v.front();
    const int size = int(v.size());
    for (int i = 1; i < size; ++i)
        std::cout << ' ' << v[i];
    std::cout << '\n';
}

static void solve(const std::vector<int>& input) { // O(NlogN)
    assert(!input.empty());
    print(input);

    std::vector<std::vector<int>> seqs; // seqs[K] is optimal so far decreasing sequence ot length K+1
    std::vector<int> scores; // scores[K] = X iff exists decreasing sequence of length K+1 ending with X

    scores.push_back(input.front());
    seqs.push_back(scores);
    for (int i = 1; i < int(input.size()); ++i) {
        const auto val = input[i];
        const auto it = std::lower_bound(scores.begin(), scores.end(), val, [](const int e, const int v){
            return v < e;
        });
        if (it == scores.end()) {
            scores.push_back(val);
            seqs.push_back(seqs.back());
            seqs.back().push_back(val);
        } else {
            *it = val;
            const int ind = it - scores.begin();
            if (ind == 0)
                seqs[0][0] = val;
            else {
                seqs[ind] = seqs[ind - 1];
                seqs[ind].push_back(val);
            }
        }
    }

    std::cout << "Maximal decreasing subsequence length is: " << scores.size() << '\n';
    print(seqs.back());
    std::cout << std::endl;
}

constexpr int size = 35;
constexpr int range = 70;

int main(int, char**)
{
    solve({10, 8, 9, 4, 6, 3});
    std::vector<int> input;
    std::generate_n(std::back_inserter(input), size, []{ return rand_in_range(range); });
    solve(input);
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 maximal_decreasing.cpp -o maximal_decreasing.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address maximal_decreasing.cpp -o maximal_decreasing

Run:
py.exe interactive_runner.py py.exe maximal_decreasing_testing_tool.py 1 -- maximal_decreasing.exe
maximal_decreasing.exe < maximal_decreasing.in

Input:


Output:


*/

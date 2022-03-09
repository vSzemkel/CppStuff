
#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>

// Maximal decreasing sequence
// Given an integer sequence find the length of maximal decreasing non-consecutive subsequence

// for counting unique max sequences see: /usaco/chapter4/buylow.cpp

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

static std::vector<int> solve(const std::vector<int>& input) { // O(NlogA) with O(N) memory
    assert(!input.empty());
    const int size = int(input.size());
    std::vector<int> tails; // tails[K] = X iff exists decreasing sequence of length K+1 ending with X
    std::vector<int> locations; // locations[K] = X iff exists decreasing sequence of length K+1 ending with input[X]
    std::vector<int> prev(size, -1); // prev[K] = X iff exists decreasing sequence of length K+1 ending with [input[X], tails[K]]

    locations.push_back(0);
    tails.push_back(input.front());
    for (int i = 1; i < size; ++i) {
        const auto val = input[i];
        if (val < tails.back()) {
            prev[i] = locations.back();
            tails.push_back(val);
            locations.push_back(i);
        } else {
            const auto it = std::lower_bound(tails.begin(), tails.end(), val, [](const int e, const int v){
                return v < e;
            });
            *it = val;
            const int ind = it - tails.begin();
            locations[ind] = i;
            prev[i] = (ind > 0) ? locations[ind - 1] : -1;
        }
    }

    std::vector<int> ans;
    for (int p = locations.back(); p >= 0; p = prev[p])
        ans.push_back(input[p]);
    std::reverse(ans.begin(), ans.end());

    return ans;
}

static std::vector<int> solve2(const std::vector<int>& input) { // O(NlogA) with O(A2) memory
    assert(!input.empty());
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

    return seqs.back();
}

constexpr int size = 35;
constexpr int range = 70;

int main(int, char**)
{
    std::vector<int> input;
    std::generate_n(std::back_inserter(input), size, []{ return rand_in_range(range); });
    print(input);

    const auto ans = solve(input);
    std::cout << "Maximal decreasing subsequence length is: " << ans.size() << '\n';
    print(ans);
    assert(ans == solve2(input));
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 maximal_decreasing.cpp -o maximal_decreasing.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address maximal_decreasing.cpp -o maximal_decreasing

*/

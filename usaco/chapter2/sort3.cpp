/*
ID: marcin.8
LANG: C++
TASK: sort3
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=bN0sJJ0P9Qj&S=sort3
*/

#include <algorithm>
#include <array>
#include <assert.h>
#include <fstream>
#include <vector>

std::ifstream task_in("sort3.in");
std::ofstream task_out("sort3.out");

int N, ans{0};

static void swap(std::vector<int>& data, const int i, const int j) {
    std::swap(data[i], data[j]);
    // for (const int i : data) std::cout << i; std::string s(N, '-'); s[i] = s[j] = '|'; std::cout << '\n' << s << std::endl;
    ++ans;
}

static void tail(std::vector<int>&& data) {
    int lo{0}, hi{N - 1};
    while (true) {
        while (lo < hi && data[lo] != 3) ++lo;
        while (lo < hi && data[hi] != 1) --hi;
        if (lo >= hi) break;
        swap(data, lo, hi);
    }

    const int mid = lo;
    lo = 0; hi = mid - 1;
    while (true) {
        while (lo < hi && data[lo] != 2) ++lo;
        while (lo < hi && data[hi] != 1) --hi;
        if (lo >= hi) break;
        swap(data, lo, hi);
    }

    lo = mid; hi = N - 1;
    while (true) {
        while (lo < hi && data[lo] != 3) ++lo;
        while (lo < hi && data[hi] != 2) --hi;
        if (lo >= hi) break;
        swap(data, lo, hi);
    }
}

int main(int, char**)
{
    task_in >> N;
    std::vector<int> data(N);
    std::array<std::vector<int>, 4> positions;
    std::array<std::pair<int, int>, 4> ranges;
    for (int i = 0; i < N; ++i) {
        int val; task_in >> val;
        data[i] = val;
        positions[val].push_back(i);
    }
    ranges[1] = {0, int(positions[1].size())};
    ranges[2] = {ranges[1].second, ranges[1].second + int(positions[2].size())};
    ranges[3] = {ranges[2].second, N};

    // swap all pairs with two correcting positions
    for (int val : {1, 2})
        for (int i = ranges[val].first; i < ranges[val].second; ++i)
            if (data[i] != val) {
                const auto& range = ranges[data[i]];
                for (int j : positions[val])
                    if (range.first <= j && j < range.second && data[j] == val) {
                        swap(data, i, j);
                        break;
                    }
            }

    // reminders are 3-cycles, each needs 2 swaps to correct
    int misplaced{0};
    for (int val : {1, 2, 3})
        for (int i = ranges[val].first; i < ranges[val].second; ++i)
            if (data[i] != val)
                ++misplaced;

    ans += misplaced * 2 / 3;

    // or finish with fail
    // tail(std::move(data));
    // assert(std::is_sorted(data.begin(), data.end()));

    task_out << ans << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 sort3.cpp -o sort3.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address sort3.cpp -o sort3

Input:

9
2
2
1
3
3
3
2
3
1

Output:


*/

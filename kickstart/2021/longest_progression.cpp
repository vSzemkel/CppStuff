
#include <iostream>
#include <numeric>
#include <vector>

// Longest progression
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000435a5b/000000000077a3a5

static void solve() {
    int64_t N; std::cin >> N;
    std::vector<int64_t> data(N);
    for (int i = 0; i < N; ++i) 
        std::cin >> data[i];
    int64_t prev = data[0], next = data[1];
    int64_t diff = next - prev;
    std::vector<int64_t> diffs(1, diff), counts(1, 2), starts(1);
    for (int i = 2; i < N; ++i) {
        prev = next;
        next = data[i];
        if (next - prev == diff)
            ++counts.back();
        else {
            diff = next - prev;
            counts.push_back(2);
            diffs.push_back(diff);
            starts.push_back(i - 1);
        }
    }

    auto ret = counts[0];
    if (ret < N) {
        const int size = (int)diffs.size();
        for (int i = 0; i < size; ++i) {
            auto ans = counts[i] + 1;
            if (i + 2 < size && counts[i + 1] == 2 && data[starts[i + 1] + 2] - data[starts[i + 1]] == 2 * diffs[i])
                ret = std::max(ret, counts[i] + 2);
            if (i + 2 < size && counts[i + 1] == 2 && data[starts[i + 2]] - data[starts[i]] == 2 * diffs[i + 2])
                ret = std::max(ret, 2 + counts[i + 2]);
            if (i + 3 < size && counts[i + 1] == 2 && counts[i + 2] == 2 && (data[starts[i + 3]] - data[starts[i + 1]]) == 2 * diffs[i] && diffs[i] == diffs[i + 3])
                ans = std::max(ans, counts[i] + counts[i + 3] + 1);

            ret = std::max(ret, ans);
        }
    }

    std::cout << ret;
}

static void solve2() {
    int64_t N; std::cin >> N;
    std::vector<int64_t> data(N);
    for (int i = 0; i < N; ++i)
        std::cin >> data[i];

    int ret{2}, i{1}, changed;
    while (i < N - 1) {
        int start{i};
        auto copy = data;
        bool corrected{false};
        const auto diff = copy[i] - copy[i - 1];
        while (i < N - 1 && copy[i + 1] - copy[i] == diff)
            ++i;
        if (i < N - 1) {
            copy[i + 1] = diff + copy[i];
            changed = i + 1;
            corrected = true;
        }
        while (i < N - 1 && copy[i + 1] - copy[i] == diff)
            ++i;
        int ans = i - start + 2;
        int ans_no_corr = corrected ? (changed - start + 1) : ans;
        if (start > 1) {
            --start;
            while (start > 0 && copy[start] - copy[start - 1] == diff) {
                ++ans_no_corr;
                ++ans;
                --start;
            }
            copy[start - 1] = copy[start] - diff;
            while (start > 0 && copy[start] - copy[start - 1] == diff) {
                ++ans_no_corr;
                --start;
            }
            ans = std::max(ans, ans_no_corr);
        }

        ret = std::max(ret, ans);
    }

    std::cout << ret;
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 longest_progression.cpp -o longest_progression.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address longest_progression.cpp -o longest_progression.o

Run:
longest_progression.exe < longest_progression.in

Input:

3
4
9 7 5 3
9
5 5 4 5 5 5 4 5 6
4
8 5 2 0

Output:


*/


#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <queue>
#include <random>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// Combination Lock
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000001a0069/0000000000414a24

int g_N;

const auto rand_in_range = [](const int ubound) {
    std::random_device seed;
    std::mt19937 gen{seed()};
    std::uniform_int_distribution<int> dist(0, ubound - 1);
    return dist(gen);
};

int distance(const int a, const int b)
{
    const int c = std::abs(a - b);
    return std::min(c, g_N - c);
}

bool crossing(const int wheel, const int target)
{
    const int c = std::abs(wheel - target);
    return g_N - c < c;
}

int64_t set_all_to(const std::vector<int64_t>& input, const int target)
{
    int64_t ret{0};
    for (const auto& w : input)
        ret += distance(w, target);
    return ret;
}

std::unordered_map<int, int64_t> cache;
int64_t set_all_to_cached(const std::vector<int64_t>& input, const int target)
{
    const auto cached = cache.find(target);
    if (cached != cache.end())
        return cached->second;

    int64_t ret{0};
    for (const auto& w : input)
        ret += distance(w, target);

    cache[target] = ret;
    return ret;
}

int64_t combination_lock(std::vector<int64_t>& input) // O(WlogW) for geniuses
{
    int64_t switch_pos{0}, prev{0}, cur{0};
    for (const auto w : input) {
        cur += distance(w, g_N);
        if (2 * w < g_N) ++switch_pos;
    }

    int64_t ret = cur;
    const auto size = input.size();
    for (int pos = 0; pos < size; ++pos) { // set all to input[pos]
        const auto target = input[pos];
        const auto delta = target - prev;
        if (delta > 0) { // this if is critical, can not ++switch_pos prematurely
            // advance down
            cur -= (switch_pos - pos) * delta;
            // transit
            while (switch_pos < input.size() && 2 * ((g_N + input[switch_pos] - target) % g_N) < g_N) {
                cur -= distance(input[switch_pos], prev);
                cur += distance(input[switch_pos], target);
                ++switch_pos;
            }
            // advance up
            cur += (input.size() - switch_pos) * delta;
        }

        prev = target;
        input.push_back(target);
        ret = std::min(ret, cur);
    }

    return ret;
}

int64_t combination_lock2(const std::vector<int64_t>& input) // O(WlogW) for cyborgs
{
    std::vector<int64_t> partial = { 0 };
    std::partial_sum(input.begin(), input.end(), std::back_inserter(partial));
    const auto weight = [&partial](const int64_t i, const int64_t j) -> int64_t { 
        return partial[j + 1] - partial[i];
    };

    const auto size = input.size();
    int64_t ret = size * g_N;
    for (int pos = 0; pos < size; ++pos) {
        while (pos + 1 < size && input[pos] == input[pos + 1])
            ++pos;

        int64_t tmp{0};
        const int64_t wheel = input[pos];
        if (pos > 0) {
            const auto ind = std::partition_point(input.begin(), input.begin() + pos, [wheel](const auto a){ return crossing(a, wheel); }) - input.begin();
            if (ind > 0) // crossing
                tmp += ind * (g_N - wheel) + weight(0, ind - 1);
            if (ind < pos)
                tmp += (pos - ind) * wheel - weight(ind, pos - 1);
        }

        if (pos < size - 1) {
            const auto ind = std::partition_point(input.begin() + pos + 1, input.end(), [wheel](const auto a){ return !crossing(a, wheel); }) - input.begin();
            if (ind > pos + 1)
                tmp += weight(pos + 1, ind - 1) - (ind - pos - 1) * wheel;
            if (ind < size) // crossing
                tmp += (size - ind) * (g_N + wheel) - weight(ind, size - 1);
        }

        ret = std::min(ret, tmp);
    }

    return ret;
}

static std::pair<int, int64_t> sinusoid_min(const std::vector<int64_t>& input, const std::pair<int, int64_t> left, const std::pair<int, int64_t> right) // {index, value}
{
    if (right.first - left.first < 2)
        return left.second < right.second ? left : right;

    const int mid = (left.first + right.first) / 2;
    const int64_t mval = set_all_to_cached(input, input[mid]);
    const auto middle = std::make_pair(mid, mval);
    if (left.second < mval && mval < right.second)
        return sinusoid_min(input, left, middle);
    if (left.second > mval && mval > right.second)
        return sinusoid_min(input, middle, right);

    const auto lcan = sinusoid_min(input, left, middle);
    const auto rcan = sinusoid_min(input, middle, right);
    return lcan.second < rcan.second ? lcan : rcan;
}

int64_t combination_lock3(const std::vector<int64_t>& input) // WRONG, it has 2 local minimums in cycle
{
    cache.clear();
    const auto size = int(input.size());
    if (size < 2) return 0;

    const auto can = sinusoid_min(input, {0, set_all_to_cached(input, input.front())}, {size - 1, set_all_to_cached(input, input.back())});
    return can.second;
}

int64_t combination_lock4(const std::vector<int64_t>& input) // O(W2)
{
    int last{-1};
    int64_t ret = input.size() * g_N;
    for (const auto target : input) {
        if (target == last) continue;
        ret = std::min(ret, set_all_to(input, target));
        last = target;
    }

    return ret;
}

void fuze()
{
    const int size = 500'000;
    auto minfail = std::numeric_limits<int64_t>::max();
    for (int n = 0; n < size; ++n) {
        g_N = 10;
        std::vector<int64_t> input(10);
        for (auto& w : input) w = 1 + rand_in_range(g_N);
        std::sort(input.begin(), input.end()); // Sort for all solutions

        const auto slow = combination_lock3(input);
        const auto fast = combination_lock(input);
        if (fast != slow && input.size() < minfail) {
            minfail = input.size();
            std::cout << minfail << " " << g_N << " [" << fast << ", " << slow << "]\n";
            for (auto w : input) std::cout << w << ' ';
            std::cout << '\n';
        }
    }
}

int main(int argc, char* argv[])
{
    //fuze(); return 1;
    // parse console input
    int no_of_cases, wheels;
    std::vector<int64_t> input;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cin >> wheels >> g_N;
        input.resize(wheels);
        for (auto& c : input) std::cin >> c;
        std::sort(input.begin(), input.end()); // Sort for all solutions
        std::cout << "Case #" << g << ": " << combination_lock3(input) << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -ggdb3 -O0 -std=c++17 combination_lock.cpp -o combination_lock.exe

Run:
combination_lock.exe < combination_lock.in

Input:

11
1 827
826
3 10
9 1 5
2 11
1 7
5 10
10 2 2 2 9
5 10
7 7 7 7 7
3 5
2 3 4
4 10
2 9 3 8
10 10
1 2 3 4 5 6 7 8 9 10
2 10
1 2
2 10
1 10
3 63
9 41 62

Output:

Case #1: 0
Case #2: 6
Case #3: 5
Case #4: 5
Case #5: 0
Case #6: 2
Case #7: 8
Case #8: 25
Case #9: 1
Case #10: 1
Case #11: 31

*/
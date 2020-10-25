
#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <queue>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

// Combination Lock
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000001a0069/0000000000414a24

int g_N;

int distance(const int a, const int b)
{
    const int c = abs(a - b);
    return std::min(c, g_N - c);
}

bool crossing(const int wheel, const int target)
{
    const int c = abs(wheel - target);
    return g_N - c < c;
}

int64_t set_all_to(const std::vector<int64_t>& input, const int target)
{
    int64_t ret{0};
    for (const auto& w : input)
        ret += distance(w, target);
    return ret;
}

int64_t combination_lock(std::vector<int64_t>& input) // O(WlogW) for cyborgs
{
    std::vector<int64_t> partial = { 0 };
    std::sort(input.begin(), input.end());
    std::inclusive_scan(input.begin(), input.end(), std::back_inserter(partial));
    const auto weight = [&partial](const int64_t i, const int64_t j) -> int64_t { 
        return partial[j + 1] - partial[i];
    };

    const auto size = input.size();
    int64_t ret = size * g_N;
    for (int pos = 0; pos < size; ++pos) {
        const int64_t wheel = input[pos];
        int64_t left{0}, right{0}, tmp{0};
        if (pos > 0) {
            int ind = std::lower_bound(input.begin(), input.begin() + pos, 0, [wheel](const int a, int){ return crossing(a, wheel); }) - input.begin();
            if (ind > 0) // crossing
                left = ind * (g_N - wheel) + weight(0, ind - 1);
            if (ind < pos)
                right = (pos - ind) * wheel - weight(ind, pos - 1);

            tmp = left + right;
        }

        if (pos < size - 1) {
            left = right = 0;
            int ind = std::lower_bound(input.begin() + pos + 1, input.end(), 0, [wheel](const int a, int){ return !crossing(a, wheel); }) - input.begin();
            if (ind > pos + 1)
                left = weight(pos + 1, ind - 1) - (ind - pos - 1) * wheel;
            if (ind < size) // crossing
                right = (size - ind) * (g_N + wheel) - weight(ind, size - 1);

            tmp += left + right;
        }

        ret = std::min(ret, tmp);
    }

    return ret;
}

// [3 4 8 9 11 14] -> [21 21 19 19 21 21] logn_min_elem fails
int64_t combination_lock2(const std::vector<int64_t>& input) // O(WlogW) for nerds
{
    // auto debug = input;
    // std::transform(input.begin(), input.end(), debug.begin(), [&input](auto w){ return set_all_to(input, w);});

    auto uniq_input = input;
    std::sort(uniq_input.begin(), uniq_input.end());
    const auto uniq_end = std::unique(uniq_input.begin(), uniq_input.end());
    if (uniq_input.size() < 2) return 0;

    auto pocz = uniq_input.begin();
    auto ret = set_all_to(input, uniq_input[0]);
    if (ret < set_all_to(input, uniq_input[1])) {
        pocz = std::lower_bound(pocz + 1, uniq_end, 0,
        [&input](const auto& n, auto) {
            const auto pred = &n - 1;
            return set_all_to(input, *pred) <= set_all_to(input, n);
        }) - 1;
    }
    const auto ind = std::lower_bound(pocz + 1, uniq_end, 0,
        [&input](const auto& n, auto) {
            const auto pred = &n - 1;
            return set_all_to(input, *pred) >= set_all_to(input, n);
        }) - 1;

    ret = std::min(ret, set_all_to(input, *ind));
    return std::min(ret, set_all_to(input, input.back()));
}

int64_t combination_lock3(const std::vector<int64_t>& input) // O(W2)
{
    int64_t ret = input.size() * g_N;
    std::unordered_set<int64_t> targets(input.begin(), input.end());
    for (const auto target : targets)
        ret = std::min(ret, set_all_to(input, target));

    return ret;
}

void fuze()
{
    const int size = 500'000;
    auto minfail = std::numeric_limits<int64_t>::max();
    std::random_device rd;
    std::uniform_int_distribution<int> dist(1, 1 << 4);
    for (int n = 0; n < size; ++n) {
        g_N = dist(rd);
        std::vector<int64_t> input(6);
        std::uniform_int_distribution<int> wdist(1, g_N);
        for (auto& w : input) w = wdist(rd);

        const auto fast = combination_lock(input);
        const auto slow = combination_lock2(input);
        if (fast != slow && input.size() < minfail) {
            minfail = input.size();
            std::cout << minfail << " " << g_N << "\n";
            for (auto w : input) std::cout << w << " ";
            std::cout << "\n";
        }
    }
}

int main(int argc, char* argv[])
{
    //fuze();
    // parse console input
    int no_of_cases, wheels;
    std::vector<int64_t> input;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cin >> wheels >> g_N;
        input.resize(wheels);
        for (auto& c : input) std::cin >> c;
        std::cout << "Case #" << g << ": " << combination_lock2(input) << "\n";
        std::cout << "Case #" << g << ": " << combination_lock(input) << "\n";
    }
}

/* clang++.exe -Wall -ggdb3 -O0 -std=c++17 combination_lock.cpp -o combination_lock.exe
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
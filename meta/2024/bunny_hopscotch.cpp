
#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <format>
#include <unordered_map>
#include <utility>
#include <vector>

// Bunny Hopscotch
// https://www.facebook.com/codingcompetitions/hacker-cup/2024/round-2/problems/C

template <typename T, typename U>
static T last_true(T lo, T hi, U f) {
    lo--;
    assert(lo <= hi); // assuming f is decreasing
    while (lo < hi) { // find last index such that f is true
        const T mid = lo + (hi - lo + 1) / 2; // this will work for negative numbers too
        f(mid) ? lo = mid : hi = mid - 1;
    }
    return lo;
}

template <typename T = int>
struct subrange_sum_t
{
    subrange_sum_t(const int size = 0) : _size(size), _data(size) {
    }

    subrange_sum_t(const std::vector<T>& data) {
        reset(data);
    }

    void reset(const std::vector<T>& data) {
        _size = data.size();
        _data.assign(_size, T{});
        for (int i = 0; i < _size; ++i) 
            for (int a = i; a < _size; a |= a + 1)
                _data[a] += data[i];
    }

    void update(const int pos, const T delta) {
        for (auto i = pos; i < _size; i |= i + 1)
            _data[i] += delta;
    }

    T query(const int pos) {
        return query(0, pos);
    }

    T query(const int i, const int j) { // closed range [i..j]
        T ret{0};
        for (int k = j + 1; k > 0; k &= k - 1)
            ret += _data[k - 1];
        for (int k = i; k > 0; k &= k - 1)
            ret -= _data[k - 1];

        return ret;
    }

    int ksum_prefix(const T value) {
        int l{0}, r{_size};
        while (l < r) {
            const auto mid = l + (r - l) / 2;
            if (query(mid) >= value)
                r = mid;
            else
                l = mid + 1;
        }
        return l; // l = min(i) : query(0, i) >= value || l == _size if total < value
    }

    int ksum_sufix(const T value) {
        int l{-1}, r{_size - 1}, total = query(r);
        while (l < r) {
            const auto mid = l + (r - l + 1) / 2;
            if (query(mid) <= total - value)
                l = mid;
            else
                r = mid - 1;
        }
        if (total >= value && l < _size - 1) ++l;
        return l; // l = max(i) : query(i, ub) >= value || l == -1 if total < value
    }

    int _size;
    std::vector<T> _data;
};

int R, C;
int64_t K;
int64_t board[803][803];
std::unordered_map<int, std::vector<std::pair<int, int>>> owns;

static int64_t how_many_valid_jumps_does_not_exceed_length(const int length) {
    int64_t ans{};
    // count all valid jumps within the range
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c) {
            const int l = std::max(0, c - length);
            const int r = std::min(C - 1, c + length);
            const int t = std::max(0, r - length);
            const int b = std::min(R - 1, r + length);
            ans += (r - l + 1) * (b - t + 1) - 1;
        }

    // exclude uniform ownership jumps
    for (const auto& [b, jumps] : owns) {
        int w{};
        subrange_sum_t owned_in_col(C);
        // const int sz = int(jumps.size());
        for (const auto [cr, cc] : jumps) {
            while (length < cr - jumps[w].first || length < cc - jumps[w].second) {
                owned_in_col.update(jumps[w].second, -1);
                ++w;
            }

            const int l = std::max(0, cc - length);
            const int r = std::min(R - 1, cc + length);
            ans -= 2 * owned_in_col.query(l, r);
            owned_in_col.update(cc, +1);
        }
    }

    return ans;
}

static int solve() {
    std::cin >> R >> C >> K;
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c) {
            auto& b = board[r][c];
            std::cin >> b;
            owns[b].emplace_back(r, c);
        }

    const auto predicate = [](int length){ return how_many_valid_jumps_does_not_exceed_length(length) <= K; };
    return last_true(1, std::max(R, C), predicate);
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g)
        std::cout << std::format("Case #{}: {}\n", g, solve());
}

/*

Format:
clang-format -i bunny_hopscotch.cpp

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 bunny_hopscotch.cpp
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address bunny_hopscotch.cpp

Run:
py.exe interactive_runner.py py.exe bunny_hopscotch_testing_tool.py 1 -- a.exe
a.exe < bunny_hopscotch.in

Input:


Output:


*/


#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

// H-Index
// 2019 https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050edd/00000000001a274e
// 2022 https://codingcompetitions.withgoogle.com/kickstart/round/00000000008f4332/0000000000941e56

template <typename T>
struct subrange_sum_t
{
    subrange_sum_t(const int size = 0) : _size(size), _data(size) {
    }

    subrange_sum_t(const std::vector<T>& data) {
        reset(data);
    }

    void reset(const std::vector<T>& data) {
        _size = data.size();
        _data.assign(_size, 0);
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

    int _size;
    std::vector<T> _data;
};

template <typename T, typename U>
static T last_true(T lo, T hi, U f) {
    lo--;
    while (lo < hi) { // find last index such that f is true
        const T mid = lo + (hi - lo + 1) / 2; // this will work for negative numbers too
        f(mid) ? lo = mid : hi = mid - 1;
    }
    return lo;
}

constexpr const int64_t MAX = 100000;

static void solve() { // O(NlogMAX)
    int N; std::cin >> N;
    int64_t max{0};
    subrange_sum_t<int64_t> partial(MAX + 1);
    const auto check = [&](const int64_t can) {
        return can <= partial.query(can, MAX);
    };

    for (; N; --N) {
        int64_t c; std::cin >> c;
        partial.update(c, 1);
        max = last_true(max + 1, MAX, check);

        std::cout << max << ' ';
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 h-index.cpp -o h-index.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address h-index.cpp -o h-index

Run:
h-index.exe < h-index.in

Input:

2
3
5 1 2
6
1 3 3 2 2 15

Output:

Case #1: 1 1 2 
Case #2: 1 1 2 2 2 3
Case #3: 1 1 2 2 2 2 2 2 3 3

*/


#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

// H-Index
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008f4332/0000000000941e56

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

constexpr const int MAX = 100000;

static void solve() {
    int N; std::cin >> N;
    int64_t max{0};
    subrange_sum_t<int64_t> partial(MAX + 1);
    for (; N; --N) {
        int64_t c; std::cin >> c;
        partial.update(c, 1);
        int64_t can = max + 1;
        while (can <= MAX && can <= partial.query(can, MAX))
            ++can;
        max = --can;

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


#include <cassert>
#include <iostream>
#include <set>
#include <utility>
#include <vector>

// Wiggle Walk
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050ff2/0000000000150aac

struct range_set_t {
    using range_t = std::pair<int, int>;

    void add(const int x) {
        auto next = _ranges.lower_bound({x + 1, 0}), prev = next;
        if (prev != _ranges.begin())
            --prev;
        if (prev != _ranges.end()) {
            if (prev->first <= x && x <= prev->second) // already in
                return;
            if (prev->second + 1 == x) { // coalesce
                if (next != _ranges.end() && x + 1 == next->first) {
                    _ranges.emplace(prev->first, next->second);
                    _ranges.erase(next);
                } else
                    _ranges.emplace(prev->first, x);
                _ranges.erase(prev);
                return;
            }
        }

        if (next != _ranges.end() && x + 1 == next->first) {
            _ranges.emplace(x, next->second);
            _ranges.erase(next);
            return;
        }

        _ranges.emplace(x, x);
    }

    auto get(const int x) {
        auto next = _ranges.lower_bound({x + 1, 0}), prev = next;
        if (prev == _ranges.begin())
            return NO_RANGE;
        --prev;
        if (prev != _ranges.end() && prev->first <= x && x <= prev->second)
            return *prev;

        return NO_RANGE;
    }

    bool check(const int x) {
        auto next = _ranges.lower_bound({x + 1, 0}), prev = next;
        if (prev == _ranges.begin())
            return false;
        --prev;
        return prev != _ranges.end() && prev->first <= x && x <= prev->second;
    }

    static constexpr const range_t NO_RANGE{-1, -1};
  private:
    std::set<range_t> _ranges;
};

static void solve() {
    std::string S;
    int N, R, C, r, c;
    std::cin >> N >> R >> C >> r >> c >> S;

    std::vector<range_set_t> rowspan(C + 1); // vertical
    std::vector<range_set_t> colspan(R + 1); // horizontal
    rowspan[c].add(r);
    colspan[r].add(c);
    range_set_t::range_t span;
    for (const char d : S) {
        switch (d) {
            case 'N':
            case 'S':
                span = rowspan[c].get(r);
                r = (d == 'N') ? span.first - 1 : span.second + 1;
                break;
            case 'E':
            case 'W':
                span = colspan[r].get(c);
                c = (d == 'W') ? span.first - 1 : span.second + 1;
                break;
        }

        colspan[r].add(c);
        rowspan[c].add(r);
    }

    std::cout << r << ' ' << c;
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 wiggle_walk.cpp -o wiggle_walk.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address wiggle_walk.cpp -o wiggle_walk

Run:
wiggle_walk.exe < wiggle_walk.in

Input:

3
5 3 6 2 3
EEWNS
4 3 3 1 1
SESE
11 5 8 3 4
NEESSWWNESE

Output:

Case #1: 3 2
Case #2: 3 3
Case #3: 3 7

*/

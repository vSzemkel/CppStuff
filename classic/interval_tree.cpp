
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

// Interval tree
// Can answer in O(logN + M) a list of intervals containing arbitrary point
// source: "CP in Python 128", page 82

template <typename T = int>
struct interval_tree_t
{
    using interval_t = std::pair<T, T>; // [i, j)

    interval_tree_t(const std::vector<interval_t>& intervals) {
        assert(std::is_sorted(intervals.begin(), intervals.end()));
        std::vector<interval_t> lf, rt;
        _center = intervals[intervals.size() / 2].first;
        for (const auto& s : intervals)
            if (s.second <= _center)
                lf.push_back(s);
            else if (_center < s.first)
                rt.push_back(s);
            else
                _by_low.push_back(s);

        _by_high = _by_low;
        std::sort(_by_high.begin(), _by_high.end(), [](const interval_t& s1, const interval_t& s2) {
            return s1.second < s2.second;
        });

        if (!lf.empty()) _left = std::make_unique<interval_tree_t>(lf);
        if (!rt.empty()) _right = std::make_unique<interval_tree_t>(rt);
    }

    std::vector<interval_t> list_containing(const T p) {
        const auto marker = interval_t{p, INF};
        std::vector<interval_t> ret;
        if (p < _center) {
            if (_left) ret = _left.get()->list_containing(p);
            const auto it = std::upper_bound(_by_low.begin(), _by_low.end(), marker);
            ret.insert(ret.end(), _by_low.begin(), it);
        } else {
            if (_right) ret = _right.get()->list_containing(p);
            const auto it = std::upper_bound(_by_high.begin(), _by_high.end(), marker, [](const interval_t& m, const interval_t& s) {
                return m.first < s.second;
            });
            ret.insert(ret.end(), it, _by_high.end());
        }

        return ret;
    }

    static const auto INF = std::numeric_limits<T>::max();

  private:
    T _center;
    std::vector<interval_t> _by_low, _by_high;
    std::unique_ptr<interval_tree_t> _left, _right;
};

int main(int, char**)
{
    using itv_t = interval_tree_t<int>;
    std::vector<itv_t::interval_t> input = {
        itv_t::interval_t{0, 9},
        itv_t::interval_t{1, 2},
        itv_t::interval_t{4, 7},
        itv_t::interval_t{6, 9},
        itv_t::interval_t{8, 9},
    };

    itv_t tree{input};
    assert(tree.list_containing(0).size() == 1);
    assert(tree.list_containing(1).size() == 2);
    assert(tree.list_containing(2).size() == 1);
    assert(tree.list_containing(3).size() == 1);
    assert(tree.list_containing(4).size() == 2);
    assert(tree.list_containing(5).size() == 2);
    assert(tree.list_containing(6).size() == 3);
    assert(tree.list_containing(7).size() == 2);
    assert(tree.list_containing(8).size() == 3);
    assert(tree.list_containing(9).size() == 0);
    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 interval_tree.cpp -o interval_tree.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address interval_tree.cpp -o interval_tree

*/

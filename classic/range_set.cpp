
#include <cassert>
#include <iostream>
#include <set>
#include <utility>

/**
 * @brief Collection of disconnected ranges
 * Ranges are closed: [l..r]
 * Renges are disconnected iff r1 < l2
 * see: /kickstart/2019/wiggle_walk.cpp
 */
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
                if (next != _ranges.end() && x + 1 == next->first) { // double coalesce
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

    void add_range(const int l, const int r) {
        int left{l}, right{r};
        auto next = _ranges.lower_bound({l + 1, 0}), left_it = next;
        bool adjusted = left_it != _ranges.begin();
        if (adjusted)
            --left_it;
        if (left_it != _ranges.end()) {
            if (left_it->first <= l && l <= left_it->second + 1)
                left = left_it->first;
            else if (adjusted)
                ++left_it;
        }


        next = _ranges.lower_bound({r + 2, 0});
        auto right_it = next;
        if (right_it != _ranges.begin())
            --right_it;
        if (right_it != _ranges.end()) {
            if (right_it->first - 1 <= r && r <= right_it->second)
                right = right_it->second;
            ++right_it;
        }

        _ranges.erase(left_it, right_it);
        _ranges.emplace(left, right);
    }

    void clear() {
        _ranges.clear();
    }

    auto get(const int x) const {
        auto next = _ranges.lower_bound({x + 1, 0}), prev = next;
        if (prev == _ranges.begin())
            return NO_RANGE;
        --prev;
        if (prev != _ranges.end() && prev->first <= x && x <= prev->second)
            return *prev;

        return NO_RANGE;
    }

    bool check(const int x) const {
        auto next = _ranges.lower_bound({x + 1, 0}), prev = next;
        if (prev == _ranges.begin())
            return false;
        --prev;
        return prev != _ranges.end() && prev->first <= x && x <= prev->second;
    }

    int sum() const {
        int ret{0};
        for (const auto& rn : _ranges)
            ret += rn.second - rn.first;
        return ret + int(_ranges.size());
    }

    static constexpr const range_t NO_RANGE{-1, -1};
  private:
    std::set<range_t> _ranges;
};

int main(int, char**)
{
    range_set_t rs;
    assert(rs.check(24) == false);
    assert(rs.get(24) == range_set_t::NO_RANGE);
    rs.add(24);
    assert(rs.sum() == 1);
    assert(rs.check(23) == false);
    assert(rs.check(24) == true);
    assert(rs.check(25) == false);
    assert((rs.get(24) == std::pair{24, 24}));
    rs.add(25);
    assert(rs.sum() == 2);
    assert(rs.check(23) == false);
    assert(rs.check(24) == true);
    assert(rs.check(25) == true);
    assert(rs.check(26) == false);
    assert((rs.get(24) == std::pair{24, 25}));
    rs.add(23);
    assert(rs.sum() == 3);
    assert(rs.check(22) == false);
    assert(rs.check(23) == true);
    assert(rs.check(24) == true);
    assert(rs.check(25) == true);
    assert(rs.check(26) == false);
    assert((rs.get(24) == std::pair{23, 25}));
    rs.add(27);
    assert(rs.sum() == 4);
    assert(rs.check(22) == false);
    assert(rs.check(23) == true);
    assert(rs.check(24) == true);
    assert(rs.check(25) == true);
    assert(rs.check(26) == false);
    assert(rs.check(27) == true);
    assert(rs.check(28) == false);
    assert((rs.get(24) == std::pair{23, 25}));
    rs.add(26);
    assert(rs.sum() == 5);
    assert(rs.check(22) == false);
    assert(rs.check(23) == true);
    assert(rs.check(24) == true);
    assert(rs.check(25) == true);
    assert(rs.check(26) == true);
    assert(rs.check(27) == true);
    assert(rs.check(28) == false);
    assert((rs.get(24) == std::pair{23, 27}));

    rs.clear();
    rs.add_range(10, 15);
    assert(rs.sum() == 6);
    rs.add_range(20, 25);
    assert(rs.sum() == 12);
    rs.add_range(17, 18);
    assert(rs.sum() == 14);
    rs.add_range(12, 23);
    assert(rs.sum() == 16);
    rs.add_range(0,  30);
    assert(rs.sum() == 31);

    rs.clear();
    rs.add_range(10, 15);
    assert(rs.sum() == 6);
    rs.add_range(20, 25);
    assert(rs.sum() == 12);
    rs.add_range(16, 19);
    assert(rs.sum() == 16);

    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 range_set.cpp -o range_set.exe

*/

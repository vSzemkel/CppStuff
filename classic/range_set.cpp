
#include <cassert>
#include <iostream>
#include <set>
#include <utility>

/**
 * @brief Collection of disconnected ranges
 * Ranges are closed: [l..r]
 * Renges are disconnected iff r1 < l2
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

int main(int, char**)
{
    range_set_t rs;
    assert(rs.check(24) == false);
    assert(rs.get(24) == range_set_t::NO_RANGE);
    rs.add(24);
    assert(rs.check(23) == false);
    assert(rs.check(24) == true);
    assert(rs.check(25) == false);
    assert((rs.get(24) == std::pair{24, 24}));
    rs.add(25);
    assert(rs.check(23) == false);
    assert(rs.check(24) == true);
    assert(rs.check(25) == true);
    assert(rs.check(26) == false);
    assert((rs.get(24) == std::pair{24, 25}));
    rs.add(23);
    assert(rs.check(22) == false);
    assert(rs.check(23) == true);
    assert(rs.check(24) == true);
    assert(rs.check(25) == true);
    assert(rs.check(26) == false);
    assert((rs.get(24) == std::pair{23, 25}));
    rs.add(27);
    assert(rs.check(22) == false);
    assert(rs.check(23) == true);
    assert(rs.check(24) == true);
    assert(rs.check(25) == true);
    assert(rs.check(26) == false);
    assert(rs.check(27) == true);
    assert(rs.check(28) == false);
    assert((rs.get(24) == std::pair{23, 25}));
    rs.add(26);
    assert(rs.check(22) == false);
    assert(rs.check(23) == true);
    assert(rs.check(24) == true);
    assert(rs.check(25) == true);
    assert(rs.check(26) == true);
    assert(rs.check(27) == true);
    assert(rs.check(28) == false);
    assert((rs.get(24) == std::pair{23, 27}));

    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 range_set.cpp -o range_set.exe

*/

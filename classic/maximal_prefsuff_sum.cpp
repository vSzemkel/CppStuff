
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

// Maximal prefix/suffix sum
// see: /kickstart/2019/diverse_subarray.cpp

template <typename T = int>
struct mpss_elem_t {
    T sum;  // sum of the elements
    T pref; // maximal prefix sum
    T suff; // maximal suffix sum
};

template <typename T = int>
struct mpss_t {
    mpss_t(const int size) : _size(size) {
        while (_offset < size) _offset *= 2;
        _segment.resize(2 * _offset);
    }

    mpss_t(const std::vector<T>& data) : mpss_t(data.size()) {
        for (int i = 0; i < _size; ++i)
            update(i, data[i]);
    }

    void reset() { _segment.assign(_segment.size(), 0); }

    mpss_elem_t<T> query() { return _segment[1]; }

    void update(int i, const int v) {
        i += _offset;
        _segment[i].sum = v;
        for (int z = i; z; z >>= 1)
            update_seg(z);
    }

  private:
    void update_seg(const int i) {
        auto& cur = _segment[i];
        if (i >= _offset)
            cur.pref = cur.suff = std::max(T{}, cur.sum);
        else {
            cur.sum = _segment[2 * i].sum + _segment[2 * i + 1].sum;
            cur.pref = std::max(_segment[2 * i].pref, _segment[2 * i].sum + _segment[2 * i + 1].pref);
            cur.suff = std::max(_segment[2 * i].suff + _segment[2 * i + 1].sum, _segment[2 * i + 1].suff);
        }
    }
    int _offset{1}, _size;
    std::vector<mpss_elem_t<T>> _segment;
};

int main(int, char**)
{
    mpss_t<> data{{1, 1, 1, 1, -7, 1, 1}};
    const auto q = data.query();
    assert(q.sum == -1 && q.pref == 4 && q.suff == 2);

    mpss_t<> data2{{1, 1, 1, 1, -3, 1, 1}};
    const auto q2 = data2.query();
    assert(q2.sum == 3 && q2.pref == 4 && q2.suff == 3);

    mpss_t<> data3{{1, 1, 1, 1, -3, 1, 1}};
    const auto q3 = data3.query();
    assert(q3.sum == 3 && q3.pref == 4 && q3.suff == 3);
    std::cout << "PASSED\n";
}

/* Compile:
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 maximal_prefsuff_sum.cpp -o maximal_prefsuff_sum.exe
*/


#include <iostream>
#include <optional>
#include <queue>
#include <random>

// Find Median from Data Stream
// https://leetcode.com/problems/find-median-from-data-stream

class MedianFinder {
  public:
    void addNum(int num) {
        if (_median) {
            if (num < _median) {
                _small.push(num);
                _large.push(_median.value());
            } else {
                _small.push(_median.value());
                _large.push(num);
            }
            _median.reset();
        } else {
            if (_small.empty()) {
                _median = num;
                return;
            }

            if (num < _small.top()) {
                _small.push(num);
                _median = _small.top();
                _small.pop();
            } else {
                _large.push(num);
                _median = _large.top();
                _large.pop();
            }
        }
    }
    
    double findMedian() {
        if (_median)
            return _median.value();

        return _small.top() + (_large.top() - _small.top()) / 2.0;
    }

  private:
    std::optional<int> _median;
    std::priority_queue<int> _small;
    std::priority_queue<int, std::vector<int>, std::greater<int>> _large;
};

static std::mt19937 g_gen{std::random_device{}()};

template <typename T = int>
static auto distribution(const T ubound) {
    std::uniform_int_distribution<T> dist(0, ubound - 1);
    return dist;
}

static constinit int RANGE = 1001;
static constinit int STEP = 10'000;
static constinit int COUNT = 1'000'000;

int main(int, char**)
{
    MedianFinder mf;
    auto dist = distribution<int>(RANGE);
    for (int i = 0; i < COUNT; ++i) {
        mf.addNum(dist(g_gen));
        if (i % STEP == 0)
            std::cout << i << ": " << mf.findMedian() << '\n';
    }
}
/*

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 find_median_from_stream.cpp -o find_median_from_stream.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address find_median_from_stream.cpp -o n_queens-ii

*/

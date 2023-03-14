
#include <algorithm>
#include <array>
#include <cstdlib>
#include <iostream>

// Moving_average
// Given a sequence of integers compute an average of the last N

template <std::int64_t W>
class moving_average_t {
  public:
    auto on_next(const int n) {
        static_assert(0 < W);
        auto& leaving = _cyclic_buffer[_current_index];
        _current_index = (_current_index + 1) % W;
        _sum = _sum - leaving + n;
        leaving = n;
        return double(_sum) / std::min(W, ++_count);
    }

  private:
    std::array<int, W> _cyclic_buffer{};
    std::int64_t _count{}, _sum{};
    std::size_t _current_index{};
};

int main(int, char**)
{
    using ma_t = moving_average_t<5>;

    ma_t ma1;
    for (int i = 0; i < 20; ++i)
        std::cout << i << ' ' << ma1.on_next(i) << '\n';

    ma_t ma2;
    for (int z = 20; z; --z)
        std::cout << z << ' ' << ma2.on_next(z) << '\n';
}

/*

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++17 moving_average.cpp -o moving_average.exe

*/

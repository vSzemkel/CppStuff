
#include <chrono>
#include <iostream>
#include <thread>

struct stopwatch_t
{
    using clock_t = std::chrono::high_resolution_clock;
    using duration_t = std::chrono::duration<double, std::milli>;

    stopwatch_t(bool calibrate = false)
    {
        if (calibrate) {
            auto mingap = duration_t::max();
            auto maxgap = duration_t::zero();
            for (int z = 85; z; --z) {
                const auto start = clock_t::now();
                const auto stop = clock_t::now();
                const auto gap = stop - start;
                if (gap.count() < mingap.count())
                    mingap = gap;
                if (maxgap.count() < gap.count())
                    maxgap = gap;
            }

            _accuracy = maxgap - mingap;
        }
        _start = clock_t::now();
    }

    ~stopwatch_t()
    {
        const auto d = clock_t::now() - _start;
        if (d.count() > std::giga::num)
            std::cout << std::chrono::duration<double>(d).count() << " sec.";
        else
            std::cout << duration_t{d}.count() << " ms.";
        if (_accuracy != duration_t::zero()) {
            std::cout << " (accuracy " << _accuracy << " ms.)";
        }
        std::cout << std::endl;
    }

  private:
    clock_t::time_point _start;
    duration_t _accuracy = duration_t::zero();
};

int main(int, char**)
{
    using namespace std::chrono_literals;
    {
        stopwatch_t sw;
        std::this_thread::sleep_for(0.1s);
    }
    {
        stopwatch_t calibrated_sw{true};
        std::this_thread::sleep_for(0.1s);
    }
}

/*
clang++.exe -Wall -ggdb3 -O3 -std=c++20 stopwatch.cpp -o stopwatch.exe
*/
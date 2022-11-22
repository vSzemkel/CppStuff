
#include <chrono>
#include <iostream>
#include <thread>

struct stopwatch_t {
    stopwatch_t() { _start = std::chrono::high_resolution_clock::now(); }
    ~stopwatch_t() {
        const auto d =  std::chrono::high_resolution_clock::now() - _start;
        if (d.count() > std::giga::num)
            std::cout << std::chrono::duration<double>(d).count() << " sec.";
        else
            std::cout << std::chrono::duration<double, std::milli>(d).count() << " ms.";
        // std::cout << std::endl;
    }
  private:
     std::chrono::high_resolution_clock::time_point _start;
};

int main(int, char**)
{
    stopwatch_t sw;
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
}


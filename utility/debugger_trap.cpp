#include <chrono>
#include <thread>

/**
 * @brief Call this in the process you want to attach debugger to
 * After attaching manually wait > 0.5s
 */
static void attach_trap() {
    using namespace std::chrono_literals; 
    auto last = std::chrono::system_clock::now(), now{last + 1s};
    while (now < last + 1500ms) {
        std::this_thread::sleep_for(1s); // set breakpoint here
        last = std::exchange(now, std::chrono::system_clock::now());
    }
}

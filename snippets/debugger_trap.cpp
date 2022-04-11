#include <chrono>
#include <thread>

/**
 * @brief Call this in the process you want to attach debugger to
 * After attaching manually change loop variable value
 */
static void attach_trap() {
    using namespace std::chrono_literals; 
    bool attached{}; 
    while (!attached)
        std::this_thread::sleep_for(1s);
}
